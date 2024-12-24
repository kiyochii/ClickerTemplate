#pragma once
#include <d3d9.h>




 namespace gui
{
	 
	constexpr int WIDTH = 500;
	constexpr int HEIGHT = 300;
	/*defines the ui size*/
	
	 inline int cps;
	 inline bool ligar;
	 inline bool rand;
	 inline bool hiden;
	 inline bool exit = true;
	
	/*define if the window is on or not*/
	
	 inline HWND window = nullptr;
	 inline WNDCLASSEXA windowClass = { };
	 inline POINTS position = {};

	 inline PDIRECT3D9 d3d = nullptr;
	 inline LPDIRECT3DDEVICE9 device = nullptr;
	 inline D3DPRESENT_PARAMETERS presentParameters = {};



	//criacao da tela win
	void CreatHwindow(const char* windowName, const char* className) noexcept;
	void DestroyHWindow() noexcept;

	//criacao device
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	//criacao e destruicao do imgui
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
	
}