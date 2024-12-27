#pragma once
#include <d3d9.h>
#include <iostream>



 namespace gui
{
	 
	constexpr int WIDTH = 500; 
	//INLINE GARANTE QUE SO HAVERA UMA INSTANCIA DE VARIAVEL, MESMO QUE O CABECALHO SEJA INCLUIDO EM VARIOS ARQUIVOS
	//AI DIMINUI O RISCO DE DUPLA DECLARACAO
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
	void CreatHwindow(const char* windowName, const char* className) ;
	void DestroyHWindow() ;

	//criacao device
	bool CreateDevice() ;
	void ResetDevice() ;
	void DestroyDevice() ;

	//criacao e destruicao do imgui
	//PARA CRIAR O IMGUI precisa do Hwindow e do Device
	void CreateImGui() ;
	void DestroyImGui() ;

	void BeginRender() ;
	void EndRender() ;
	void Render() ;
	
}