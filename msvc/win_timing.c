
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <time.h>
#include <psapi.h>
#include <errno.h>

#include "win_timing.h"

double  seconds_per_cycle = 0.0; /* seconds per processor cycle */
double  seconds_per_tick;        /* seconds per clock tick      */
int     need_cycles = 1;

char    speed_time_string[] = { "Windows Performance Counter" };
int     speed_precision = 100000;
double  speed_cycletime;
static double start;

#if defined( _WIN64 )

#  define current_processor_number GetCurrentProcessorNumber

#else

unsigned long current_processor_number(void)
{
    __asm
    {
	    mov     eax,1
	    cpuid
	    shr     ebx,24
	    mov     eax, ebx
    }
}

#endif

int lock_thread_to_core(void)
{   DWORD_PTR afp, afs;

	if(GetProcessAffinityMask(GetCurrentProcess(), &afp, &afs))
	{
		afp &= (DWORD_PTR)(1 << current_processor_number());
		if(SetThreadAffinityMask(GetCurrentThread(), afp))
			return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int unlock_thread_from_core(void)
{   DWORD_PTR afp, afs;

    if(GetProcessAffinityMask(GetCurrentProcess(), &afp, &afs))
	{
		if(SetThreadAffinityMask(GetCurrentThread(), afp))
			return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

void set_timing_cycles(void)
{
    need_cycles = 1;
    speed_cycletime = 1.0;
    speed_precision = 100000;
}

void set_timing_seconds(void)
{
    need_cycles = 0;
    speed_cycletime = seconds_per_cycle;
    speed_precision = 1000;
}

void init_timing(void)
{   unsigned long long cycles;
    LARGE_INTEGER ll;

    lock_thread_to_core();
    cycles = __rdtsc();
    Sleep(1000);
    cycles = __rdtsc() - cycles;
    unlock_thread_from_core();
    seconds_per_cycle = 1.0 / (double)cycles;
    QueryPerformanceFrequency(&ll);
    seconds_per_tick = 1.0 / (double)ll.QuadPart;
    QueryPerformanceCounter(&ll);
    start = seconds_per_tick * ll.QuadPart;
		set_timing_seconds();
}

void start_timing(void)
{
    if(seconds_per_cycle == 0.0)
        speed_time_init();

    if(need_cycles)
    {
        lock_thread_to_core();
        start = __rdtsc();
    }
    else
    {   LARGE_INTEGER ll;
        QueryPerformanceCounter(&ll);
        start = seconds_per_tick * ll.QuadPart;
    }
}

double end_timing(void)
{
    if(need_cycles)
    {   double t = (double)__rdtsc() - start;
        unlock_thread_from_core();
        return t;
    }
    else
    {   LARGE_INTEGER ll;
        QueryPerformanceCounter(&ll);
        return seconds_per_tick * ll.QuadPart - start;
    }
}

int get_processor_info(char *cpu_id, char *cpu_name, double *cycles_per_second)
{
    DWORD   mhz, bsize;
    HKEY    hKey;

    if(RegOpenKeyEx( HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hKey ) != ERROR_SUCCESS)
        return EXIT_FAILURE;

    bsize = 4;
    if(RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE) &mhz, &bsize) != ERROR_SUCCESS)
        return EXIT_FAILURE;

    bsize = _MAX_PATH;
    if(RegQueryValueEx(hKey, "Identifier", NULL, NULL, cpu_id, &bsize) != ERROR_SUCCESS)
        return EXIT_FAILURE;

    bsize = _MAX_PATH;
    if(RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, cpu_name, &bsize) != ERROR_SUCCESS)
        return EXIT_FAILURE;

    *cycles_per_second = 1.0e6 * mhz;
    return EXIT_SUCCESS;
}

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    FILETIME        ft;
    LARGE_INTEGER   li;
    __int64         t;
    static int      tzflag;

    if(tv)
    {
        GetSystemTimeAsFileTime(&ft);
        li.LowPart  = ft.dwLowDateTime;
        li.HighPart = ft.dwHighDateTime;
        t  = li.QuadPart;
        t -= EPOCHFILETIME;
        t /= 10;
        tv->tv_sec  = (long)(t / 1000000);
        tv->tv_usec = (long)(t % 1000000);
    }

    if (tz)
    {
        if (!tzflag)
        {
            _tzset();
            tzflag++;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }

    return 0;
}

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

#if 0

int main(void)
{   char b1[_MAX_PATH], b2[_MAX_PATH];
    unsigned long long cps;

    if(get_processor_info(b1, b2, &cps) == EXIT_SUCCESS)
    {
        speed_time_init();

        printf("\n%s", b1);
        printf("\n%s", b2);
        printf("\nSpeed: %lld", cps);
        printf("\nSpeed: %.0f", 1.0 / seconds_per_cycle);
        return 0;
    }
    else
        return EXIT_FAILURE;
}

#endif
