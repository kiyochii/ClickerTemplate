#include"gui.h"
#include"clicker.h"
#include <thread>
#include <stdio.h>
#include<iostream>


//int __stdcall wWinMain depois quando eu quiser mudar
int main(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	PWSTR arguments,
	int commandShow)
{
	HWND console;
	HWND ui;
	bool showing = true;

	std::string confirmar;
	std::cout << "Windows DLL error press any key to close ";
	std::cin >> confirmar;
	if (confirmar == "teste") {

		gui::CreatHwindow("winname", "winclassname");
		gui::CreateDevice();
		gui::CreateImGui();
		ui = FindWindowA("winclassname", NULL);
		Clicker clickerInstance;
		std::thread clickerThread(&Clicker::thread, &clickerInstance);
		clickerThread.detach();

		FreeConsole();

		while (gui::exit) {

			gui::BeginRender();
			gui::Render();
			gui::EndRender();
			if (gui::hiden == true) {
				ShowWindow(ui, SW_HIDE);
				Sleep(30);
			}
			Sleep(10);
		}
		gui::DestroyImGui();
		gui::DestroyDevice();
		gui::DestroyHWindow();


		return EXIT_SUCCESS;
	}
}
