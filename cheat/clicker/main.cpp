#include "../UI/gui.h"
#include "../UI/loadui.h"
#include "clicker.h"

#include <thread>
#include <stdio.h>
#include <iostream>


#include <windows.h>
#include <iostream>

#include <windows.h>
#include <iostream>
#include <fstream>


//int __stdcall wWinMain(

//depois mudar em: /subsystem:windows /ENTRY:mainCRTStartu
int main(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{		

	HWND ui;
	bool showing = true;
		


		gui::CreatHwindow("winname", "winclassname");
		gui::CreateDevice();
		gui::CreateImGui();
		ui = FindWindowA("winclassname", NULL);
		loadui loaduiInstance;
		Clicker clickerInstance;
		printf("oi2");
		//std::thread loaduiThread(&loadui::thread, &loaduiInstance);
		std::thread clickerThread(&Clicker::thread, &clickerInstance);
		//detach na thread
		clickerThread.detach();
		loaduiInstance.thread();
		//FreeConsole();
		//return EXIT_SUCCESS;
	
}
