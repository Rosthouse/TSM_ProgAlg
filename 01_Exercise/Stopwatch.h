//
// Created by Patrick Joos on 08.03.2018.
//

#ifndef INC_01_EXERCISE_STOPWATCH_H
#define INC_01_EXERCISE_STOPWATCH_H

#include <chrono>

using namespace std::chrono;

class Stopwatch {
private:
    high_resolution_clock clock;

    duration start;
    duration stop;
public:

    Stopwatch() {
        start = NULL;
        stop = NULL;
    }

    void Start() {
        start = clock.now().time_since_epoch();
    }

    void Stop() {
        stop = clock.now().time_since_epoch();
    }

    int GetElapsedTimeMilliseconds() {
        std::chrono::duration ticks = stop - start;
        return ticks.count();
    }
};

#endif //INC_01_EXERCISE_STOPWATCH_H
