#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>
#include <stdbool.h>

typedef struct
{
    LARGE_INTEGER StartingCount;
    LARGE_INTEGER EndingCount;
    LARGE_INTEGER TotalCount;
    LARGE_INTEGER Frequency;
    double        ElapsedMilliSeconds;
} timer;

inline void Timer_Start(timer *Timer)
{
    QueryPerformanceCounter(&Timer->StartingCount);
}

inline bool Timer_Init(timer *Timer)
{
    memset(Timer, 0, sizeof(timer)); // init to 0

    if (!QueryPerformanceFrequency(&Timer->Frequency))
    {
        return false;
    }

    Timer_Start(Timer);

    return true;
}

inline void Timer_Update(timer *Timer)
{
    // get current tick
    QueryPerformanceCounter(&Timer->EndingCount);

    // get tick count
    Timer->TotalCount.QuadPart = Timer->EndingCount.QuadPart - Timer->StartingCount.QuadPart;

    // for precision
    Timer->TotalCount.QuadPart *= 1000000;
    Timer->TotalCount.QuadPart /= Timer->Frequency.QuadPart;

    // calculate elapsed milliseconds
    Timer->ElapsedMilliSeconds = (double)Timer->TotalCount.QuadPart / 1000.0;

    QueryPerformanceCounter(&Timer->StartingCount);
}

#endif // __TIMER_H__