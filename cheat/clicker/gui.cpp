#include "gui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;
	switch (message)
	{//caso eu queira colocar isso depois, por enqt esse switch n vai servir para nada
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)/*verifica se a tela ta aberta*/
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU) //disable alt aplicationmenu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;
	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;
	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);

		}return 0;

	}

	}
	return DefWindowProcW(window, message, wideParameter, longParameter);
}

					
void gui::CreatHwindow(
	const char* windowName,
	const char* className) noexcept {

	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowExA(
		WS_EX_LAYERED,
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	SetLayeredWindowAttributes(window, RGB(0, 0, 0), 100,ULW_COLORKEY);
	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

}


	void gui::DestroyHWindow() noexcept {

		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	}

bool gui::CreateDevice() noexcept {

	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d)
		return false;
	ZeroMemory(&presentParameters, sizeof(presentParameters));
	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0) 
		return false;
	return true;

}
void gui::ResetDevice() noexcept {

	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = device->Reset(&presentParameters);
	
	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}
void gui::DestroyDevice() noexcept {

	if (device) {
		device->Release();
		device = nullptr;

	}
	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}



void gui::DestroyImGui() noexcept {

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void gui::BeginRender() noexcept {

	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
void gui::EndRender() noexcept {

	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);



	if (device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();

}


void  gui::CreateImGui() noexcept {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 10.0f; // Define o n�vel de arredondamento das bordas (valor em pixels)

	io.IniFilename = NULL;
	ImGui::StyleColorsDark();
	ImVec4 backgroundColor = ImVec4(23 / 255.0f, 94 / 255.0f, 120 / 255.0f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = backgroundColor;




	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

}
void gui::Render() noexcept {

	ImGui::SetNextWindowPos({0, 0 });

	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"x ",
		&exit,//se falso a janela fecha
		ImGuiWindowFlags_NoResize|
		ImGuiWindowFlags_NoSavedSettings|
		ImGuiWindowFlags_NoCollapse|
		ImGuiWindowFlags_NoMove	
	);

	ImGuiIO& io = ImGui::GetIO();
	ImFont* font_title = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Roboto-Bold.ttf", 23.0f, NULL, io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(title != NULL);
	ImFont* font_body = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Roboto-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesDefault());
	IM_ASSERT(body != NULL);
	ImGui::PushFont(font_title);
	ImGui::Checkbox("on", &ligar); 
	
	ImGui::Checkbox("random", &rand);
	ImGui::Text("It is highly recommended using the random function and the cps\nbellow 14 in servers with anticheat");

	ImGui::SliderInt("cps", &cps, 1.f, 20.f);

	if(ImGui::Button("Hide") == true) {
		hiden = true;
	}
	ImGui::Text("Press Insert to unhide it");
	ImGui::End();
}
