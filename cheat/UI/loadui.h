#pragma once

#include <thread>
#include "Windows.h"
#include "iostream"
#include <stdio.h>

class loadui
{

private: 
	HWND Window;
	int cont;
public:
	void thread();

};

