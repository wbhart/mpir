/*  This file is part of the MPIR Library.

    The MPIR Library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or (at
    your option) any later version.
    The MPIR Library is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
    License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with the MPIR Library; see the file COPYING.LIB.  If not, write
    to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <psapi.h>
#include <errno.h>

#include "getrusage.h"

typedef union file_t
{   FILETIME ft;
    long long lt;
} file_t;

int getrusage(int who, rusage *usage)
{
    HANDLE proc_hand;
    file_t c_time, x_time, s_time, u_time;
    int cb = 0, err = -1;

    if(who != RUSAGE_SELF) 
    {
        errno = (who == RUSAGE_CHILDREN ? ENODATA : EINVAL); 
        return err;
    }

    proc_hand = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

    if(GetProcessTimes(proc_hand, &(c_time.ft), &(x_time.ft), &(s_time.ft), &(u_time.ft))) 
    {
        PROCESS_MEMORY_COUNTERS ctrs;
        
        /* The units returned by GetProcessTimes are 100 nanoseconds */
        u_time.lt = (u_time.lt + 5) / 10;
        s_time.lt = (s_time.lt + 5) / 10;

        usage->ru_utime.tv_sec  = (long)(u_time.lt / 1000000ll);
        usage->ru_stime.tv_sec  = (long)(s_time.lt / 1000000ll);
        usage->ru_utime.tv_usec = (long)(u_time.lt % 1000000ll);
        usage->ru_stime.tv_usec = (long)(s_time.lt % 1000000ll);

        if(GetProcessMemoryInfo(proc_hand, &ctrs, sizeof(ctrs))) 
        {
            PERFORMANCE_INFORMATION perf_info;
            GetPerformanceInfo(&perf_info, sizeof(perf_info));
            usage->ru_maxrss = (DWORD) (ctrs.WorkingSetSize / perf_info.PageSize);
            usage->ru_majflt = ctrs.PageFaultCount;
            err = 0;
        }
    }

    if(err)
        errno = EACCES;
    CloseHandle(proc_hand);
    return err;
}
