#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdbool.h>
#include <windows.h>


typedef struct
{
    LARGE_INTEGER Start;
    LARGE_INTEGER Ending;
    LARGE_INTEGER Total;
    LARGE_INTEGER Frequency;
    double        ElapsedMilliSeconds;
} timer;

inline void Timer_Start(timer *Timer)
{
    QueryPerformanceCounter(&Timer->Start);
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
    QueryPerformanceCounter(&Timer->Ending);

    // get tick count
    Timer->Total.QuadPart = Timer->Ending.QuadPart - Timer->Start.QuadPart;

    // for precision
    Timer->Total.QuadPart *= 1000000;

    // calculate elapsed milliseconds
    Timer->ElapsedMilliSeconds = (double)(Timer->Total.QuadPart / Timer->Frequency.QuadPart) / 1000;
}

#endif // __TIMER_H__