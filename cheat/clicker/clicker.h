#pragma once
#include <random>
#include <chrono>
#include <Windows.h>
#include <thread>

class Clicker {
public:
    int cps;
    void thread();

private:
    void click();
    void preciseSleep(double durationInSeconds);
    double generateRandom();
};
