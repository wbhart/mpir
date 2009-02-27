
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winperf.h>
#include <Psapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "getrusage.h"

#ifndef ENODATA
#define ENODATA 61
#endif

/* The function GetPerformanceInfo() may or may not exist (e.g. Windows 2003 has it) in Psapi.dll */
typedef BOOL(*pfnGetPerformanceInfo) (PPERFORMANCE_INFORMATION pPerformanceInformation, DWORD cb);

int getrusage(int who, struct rusage * usage)
{
    HANDLE process_handle;
    HMODULE hm;
    FILETIME create_time, exit_time, kernel_time, user_time;
    PROCESS_MEMORY_COUNTERS pmc;
    DWORD cb = 0;
    size_t page_size = 8192;
    PPERFORMANCE_INFORMATION PerformanceInformation = {0};
    pfnGetPerformanceInfo fnGetPerformanceInfo = 0;

    if(hm = GetModuleHandle("Psapi.DLL"))
        fnGetPerformanceInfo = (pfnGetPerformanceInfo) GetProcAddress(hm, "GetPerformanceInfo");

    if(who != RUSAGE_SELF) 
    {
        errno = (who == RUSAGE_CHILDREN ? ENODATA : EINVAL);
        return -1;
    }

    process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

    if(GetProcessTimes(process_handle, &create_time, &exit_time, &kernel_time, &user_time)) 
    {
        long long u_time = (*(long long *) & user_time);
        long long k_time = (*(long long *) & kernel_time);
        usage->ru_utime.tv_sec = (long) (u_time / 1000000);
        usage->ru_stime.tv_sec = (long) (k_time / 1000000);
        usage->ru_utime.tv_usec = (long) (u_time % 1000000);
        usage->ru_stime.tv_usec = (long) (k_time % 1000000);

        if(fnGetPerformanceInfo && fnGetPerformanceInfo(PerformanceInformation, cb)) 
            page_size = PerformanceInformation->PageSize;

        if(GetProcessMemoryInfo(process_handle, &pmc, sizeof(pmc))) 
        {
            usage->ru_maxrss = (DWORD) (pmc.WorkingSetSize / page_size);
            usage->ru_majflt = pmc.PageFaultCount;
            if(hm) 
                FreeLibrary(hm);
            CloseHandle(process_handle);
            return 0;
        } 
    } 

    if(hm) 
        FreeLibrary(hm);
    CloseHandle(process_handle);
    errno = EACCES;
    return -1;
}
