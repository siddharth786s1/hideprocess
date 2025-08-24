/**
 * DLL Injector for Windows Process Hiding - Educational Purpose Only
 * 
 * This utility injects the process hiding DLL into target processes
 * like Task Manager, Process Explorer, etc.
 */

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <stdio.h>

static void print_env_report(void) {
    SYSTEM_INFO si = {0};
    GetNativeSystemInfo(&si);
    wprintf(L"OS native processor architecture: %u\n", si.wProcessorArchitecture);
#ifdef _WIN64
    wprintf(L"Build: x64 (_WIN64 defined)\n");
#else
    wprintf(L"Build: NOT x64 (_WIN64 NOT defined)\n");
#endif
}

int wmain(int argc, wchar_t** argv) {
#ifndef _WIN64
    fwprintf(stderr, L"ERROR: win_injector must be compiled for x64.\n");
    fwprintf(stderr, L"Open 'x64 Native Tools Command Prompt for VS 2022' and run build_x64.bat\n");
    print_env_report();
    return 0x32;
#else
    wprintf(L"win_injector diagnostics build (x64)\n");
    print_env_report();
    if (argc < 2) {
        fwprintf(stderr, L"Usage: %ls <process_name>\n", argv[0]);
        return 2;
    }
    wprintf(L"Target process (diagnostic only): %ls\n", argv[1]);
    wprintf(L"No injection is performed in this build.\n");
    return 0;
#endif
}
