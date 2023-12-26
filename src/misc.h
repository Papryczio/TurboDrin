#include "Arduino.h"

// Calibrate based on pump efficiency -- in case when source current is too low to handle both pumps working at it's full speed uncomment separate scalings.
#define TIME_SCALE 0.2
// #define TIME_SCALE_1 0.02
// #define TIME_SCALE_2 0.03

double microSecondToSeconds(long time);
int secondsToMicroSeconds(double time);
double calculateTime(int mililiters);
void clearTimer(hw_timer_t *timer);

double microSecondToSeconds(long time)
{
    return time / 1000000;
}

int secondsToMicroSeconds(double time)
{
    return (int)(time * 1000000);
}

double calculateTime(int mililiters)
{
    return ((double)mililiters * TIME_SCALE);
}

void clearTimer(hw_timer_t *timer)
{
    timerDetachInterrupt(timer);
    timerEnd(timer);
}