#include "Arduino.h"

int microSecondToSeconds(long time) {
    return time/1000000;
}

int secondsToMicroSeconds(int time) {
    return time*1000000;
}