#pragma once

#include "gui.h"
#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

namespace click {
    int cps;
    int n = 0;
   
    void precise_sleep(double duration_in_seconds) {
        auto duration = std::chrono::duration<double>(duration_in_seconds);
        auto start = std::chrono::high_resolution_clock::now();
        while (std::chrono::high_resolution_clock::now() - start < duration) {
            // Busy-wait loop
        }
    }
    double random () {

        // Cria um gerador de números aleatórios usando uma semente baseada no relógio do sistema
        std::random_device rd;
        std::mt19937 gen(rd()); // Mersenne Twister 19937 como o algoritmo gerador

        // Define os limites do intervalo desejado
        double min = 1.0;
        double max = 500.0;

        // Cria uma distribuição uniforme dentro do intervalo especificado
        std::uniform_real_distribution<double> distrib(min, max);
        double randomValue = distrib(gen);
        return randomValue;

    }
    void clicar() {
        
        if (cps != gui::cps)
            cps = gui::cps;
        double slp = 1.0 / double(cps);
        if (GetAsyncKeyState(VK_LBUTTON)) {
            HWND Window = GetForegroundWindow(); // Get Window
            if (FindWindowA("LWJGL", nullptr) == GetForegroundWindow() || FindWindowA("GLFW30", nullptr) == GetForegroundWindow()) {
               
                SendMessageW(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                SendMessageW(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
                printf("%lf", random());
    
                if (gui::rand == true) {
                    slp += 1 / random();
                    printf("randomligado");

                }
                precise_sleep(slp/2);
            }
        }
    }
    void acionar() {
       
        if (n == 1 && GetAsyncKeyState(0x58)) {
            n = 0;
            gui::ligar = false;
            Sleep(500);

        } 
        if (GetAsyncKeyState(0x58) && n == 0) {
            n = 1;
            gui::ligar = true;
            Sleep(500);
        }
    }
    void esconderGui() {
        
       
        
    }
}