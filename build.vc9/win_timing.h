
#ifndef _WIN_TIMING_H
#define _WIN_TIMING_H

#include <stdlib.h>
#include <intrin.h>
#pragma intrinsic(__rdtsc)

#if defined(__cplusplus)
extern "C"
{
#endif

#define EPOCHFILETIME (116444736000000000LL)

struct timezone 
{
    int tz_minuteswest; /* minutes W of Greenwich */
    int tz_dsttime;     /* type of dst correction */
};

struct timeval 
{
    long    tv_sec;     /* seconds               */
    long    tv_usec;    /* and microseconds      */
};

#define ENODATA         61
#define	RUSAGE_SELF	     0
#define	RUSAGE_CHILDREN	-1

typedef struct rusage
{
    struct timeval  ru_utime;   /* user time used                      */
    struct timeval  ru_stime;   /* system time used                    */
    long     ru_maxrss;  /* integral max resident set size      */
    long     ru_ixrss;   /* integral shared text memory size    */
    long     ru_idrss;   /* integral unshared data size         */
    long     ru_isrss;   /* integral unshared stack size        */
    long     ru_minflt;  /* page reclaims                       */
    long     ru_majflt;  /* page faults                         */
    long     ru_nswap;   /* swaps                               */
    long     ru_inblock; /* block input operations              */
    long     ru_oublock; /* block output operations             */
    long     ru_msgsnd;  /* messages sent                       */
    long     ru_msgrcv;  /* messages received                   */
    long     ru_nsignals;/* signals received                    */
    long     ru_nvcsw;   /* voluntary context switches          */
    long     ru_nivcsw;  /* involuntary context switches        */
} rusage;

__inline unsigned long long get_tsc(void)
{
    return __rdtsc();
}

extern char speed_time_string[];
extern double  speed_cycletime;
extern double  speed_unittime;
extern int     speed_precision;

int lock_thread_to_core(void);
int unlock_thread_from_core(void);
void speed_time_init(void);
void speed_starttime(void);
double speed_endtime(void);
void speed_cycletime_need_cycles(void);
void speed_cycletime_need_seconds(void);

int get_processor_info(char *cpu_id, char *cpu_name, double *cycles_per_second);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int getrusage(int who, rusage *usage);

#if defined(__cplusplus)
}
#endif

#endif
