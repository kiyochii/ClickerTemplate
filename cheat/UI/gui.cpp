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
	const char* className)  {

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


	void gui::DestroyHWindow()  {

		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	}

bool gui::CreateDevice()  {

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
void gui::ResetDevice()  {

ImGui_ImplDX9_InvalidateDeviceObjects();
const auto result = device->Reset(&presentParameters);

if (result == D3DERR_INVALIDCALL)
IM_ASSERT(0);
ImGui_ImplDX9_CreateDeviceObjects();
}
void gui::DestroyDevice() {

	if (device) {
		device->Release();
		device = nullptr;

	}
	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}



void gui::DestroyImGui() {

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void gui::BeginRender() {

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
void gui::EndRender() {

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


void  gui::CreateImGui() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/AGENCYR.TTF", 18.0f); // Carrega a fonte Arial

	ImGuiStyle& style = ImGui::GetStyle();

	io.IniFilename = NULL;
	ImGui::StyleColorsDark();
	//ImVec4 backgroundColor = ImVec4(176 / 255.0f, 12 / 255.0f, 223 / 255.0f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.14f, 0.18f, 1.0f); 
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       
	style.Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);     
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.6f, 0.9f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1f, 0.4f, 0.7f, 1.0f);  
	style.Colors[ImGuiCol_WindowBg] = ImVec4(53 / 255.0f, 51 / 255.0f, 62 / 255.0f, 1.0f);
	style.WindowRounding = 20.0f;

//	style.Colors[ImGuiCol_WindowBg] = backgroundColor;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

}

void gui::Render() {
	const float buttonHeight = HEIGHT / 5.0f; // Altura dos botões para ocupar exatamente a altura total da janela
	static int selectedOption = 0;           // Controle da opção selecionada

	// Configura a janela principal
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Main Window",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoScrollWithMouse
	);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 windowSize = ImGui::GetWindowSize();

	ImGui::SetCursorPos(ImVec2(20, 50));

	ImGui::BeginChild("ButtonPanel", ImVec2(100, HEIGHT), false);

	// Botao 1

	if (selectedOption == 0) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(103/255.0f , 101/255.0f, 106/255.0f, 1.0f)); // Vermelho escuro para botão ativo
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(81 / 255.0f, 55 / 255.0f, 163 / 255.0f, 1.0f)); // Vermelho escuro para botão ativo
	}
	if (ImGui::Button("Clicker", ImVec2(90, buttonHeight - 10))) {
		selectedOption = 0;
	}
	ImGui::PopStyleColor();

	// Botao 2
	if (selectedOption == 1) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(103 / 255.0f, 101 / 255.0f, 106 / 255.0f, 1.0f)); // Vermelho escuro para botão ativo

	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(81 / 255.0f, 55 / 255.0f, 163 / 255.0f, 1.0f)); // Vermelho escuro para botão ativo

	}
	if (ImGui::Button("Reach", ImVec2(90, buttonHeight - 5))) {
		selectedOption = 1;
	}
	ImGui::PopStyleColor();

	// Botao 3
	if (selectedOption == 2) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(103 / 255.0f, 101 / 255.0f, 106 / 255.0f, 1.0f)); // Vermelho escuro para botão ativo
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(81 / 255.0f, 55 / 255.0f, 163 / 255.0f, 1.0f)); // Vermelho escuro para botão ativo
	}
	if (ImGui::Button("Fast Bridge", ImVec2(90, buttonHeight - 5))) {
		selectedOption = 2;
	}
	ImGui::PopStyleColor();

	ImGui::EndChild();

	// linha
	ImVec2 lineStart = ImVec2(windowPos.x + 120, windowPos.y);
	ImVec2 lineEnd = ImVec2(windowPos.x + 120, windowPos.y + windowSize.y);
	drawList->AddLine(lineStart, lineEnd, IM_COL32(255, 255, 255, 255), 1.0f);

	ImGui::SameLine();

	ImGui::BeginChild("MainContent", ImVec2(WIDTH - 100, HEIGHT), false);

	switch (selectedOption) {
	case 0:
		ImGui::Checkbox("On", &ligar);
		if(ImGui::Button("Set Clicker Key Bind")) 
		{


		}
		ImGui::SameLine();
		ImGui::Text("A");
		ImGui::Checkbox("Random", &rand);
		ImGui::SliderInt("Cps", &cps, 1.f, 20.f);
		if (ImGui::Button("Hide") == true)
		{
			hiden = true;
		}
		if (ImGui::Button("Set Hide Key Bind"));
		ImGui::SameLine();
		ImGui::Text("A");
		break;
	case 1:
		ImGui::Text("Implementar");
		break;
	case 2:
		ImGui::Text("Implementar");
		break;
	}

	ImGui::EndChild();

	ImGui::End();
}

