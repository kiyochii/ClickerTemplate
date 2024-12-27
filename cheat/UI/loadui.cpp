#include "loadui.h"
#include "gui.h"




void loadui:: thread() 
{

	cont = 0;
	//definido na main
	loadui::Window = FindWindowA("winclassname", NULL);
	while (gui::exit) {

		gui::BeginRender();
		gui::Render();
		gui::EndRender();
		Sleep(10);
		if (gui::hiden)
		{
			ShowWindow(loadui::Window, SW_HIDE);
			cont = 1;
		} else if (cont == 1) {
			ShowWindow(loadui::Window, SW_SHOW);
			cont = 0;
		}


	}
	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();


}