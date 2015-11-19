
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

/* Windows Timing Using The Performance & Time Stamp Counters */

extern double  seconds_per_cycle;
extern double  seconds_per_tick;

int lock_thread_to_core(void);
int unlock_thread_from_core(void);
void init_timing(void);
void start_timing(void);
double end_timing(void);
void set_timing_cycles(void);
void set_timing_seconds(void);

/* Map to Linux Timing Interface */
extern char speed_time_string[];
extern int     speed_precision;
extern double  speed_cycletime;
#define speed_unittime                  seconds_per_tick
#define speed_time_init                 init_timing
#define speed_starttime                 start_timing
#define speed_endtime                   end_timing
#define speed_cycletime_need_cycles     set_timing_cycles
#define speed_cycletime_need_seconds    set_timing_seconds

int get_processor_info(char *cpu_id, char *cpu_name, double *cycles_per_second);
int gettimeofday(struct timeval *tv, struct timezone *tz);
int getrusage(int who, rusage *usage);

#if defined(__cplusplus)
}
#endif

#endif
