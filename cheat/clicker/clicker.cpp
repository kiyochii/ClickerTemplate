#include "clicker.h"
#include "../UI/gui.h"
#include <cstdio> 


void Clicker::thread() {

    SetThreadPriority(std::this_thread::get_id, THREAD_PRIORITY_TIME_CRITICAL);

    while (true) {
        if (gui::ligar) { 
            Clicker::click();
            double interval = 1.0 / static_cast<double>(gui::cps);
            auto sleep_duration = std::chrono::duration<double>(interval);
            std::this_thread::sleep_for(sleep_duration);
            // std::this_thread::sleep_for(std::chrono::duration<double>(interval));
            //TODO: ARRUMAR ISSO LOGOOOOOO
        }
        else { 
            Sleep(10);
            //nao explodir a cpu
        }
    }
}



void Clicker::preciseSleep(double durationInSeconds) {
    auto duration = std::chrono::duration<double>(durationInSeconds);
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - start < duration) {
        
    }
}

double Clicker::generateRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    double min = 1.0;
    double max = 500.0;
    std::uniform_real_distribution<double> distrib(min, max);
    return distrib(gen);
}

void Clicker::click() {
    if (cps != gui::cps) {
        cps = gui::cps;
    }
    double slp = 1.0 / double(cps);
    if (GetAsyncKeyState(VK_LBUTTON)) {
        HWND window = GetForegroundWindow();
        if (FindWindowA("LWJGL", nullptr) == window || FindWindowA("GLFW30", nullptr) == window) {
            SendMessageW(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
            SendMessageW(window, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
            printf("%lf\n", generateRandom());

            if (gui::rand) {
                slp += 1 / generateRandom();
                printf("Random ativado\n");
            }
            preciseSleep(slp / 2);
        }
    }
}
