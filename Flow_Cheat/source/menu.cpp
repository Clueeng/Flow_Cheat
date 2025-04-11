#include "menu.h"
#include "constant.h"
#include "GL/GL.h"
#include "GL/GLU.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include "imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"
#include <iostream>
#include "esp.h"
#include "settings.h"

const ImVec2 windowSize = ImVec2(400, 300);
bool showMenu = false;
bool initialized = false;
bool contextCreated = false;
HWND gameWindow;
HGLRC myContext;
HGLRC gameContext;
HDC globalDC;
WNDPROC oGameWndProc = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK NewWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (showMenu) {
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
			return 0;
		}
		switch (msg) {
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_CHAR:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP: {
				return 0;
			}
		}
	}
	return CallWindowProc(oGameWndProc, hwnd, msg, wParam, lParam);
}

void Menu::ToggleMenu()
{
	OutputDebugStringA("Pressed key for ImGUI");
	showMenu = !showMenu;

	oSetRelativeMouseMode(!showMenu);
	
	ImGuiIO& io = ImGui::GetIO();
	if (!ImGui::GetCurrentContext()) {
		OutputDebugStringA("ImGui context not created!");
		return;
	}
	io.WantCaptureMouse = showMenu;
	io.WantCaptureKeyboard = showMenu;
	io.MouseDrawCursor = showMenu;
	OutputDebugStringA("Unreleased mouse");
}

void Menu::Init()
{
	ImGui::CreateContext();
	gameWindow = FindWindowA(NULL, "AssaultCube");
	oGameWndProc = (WNDPROC)SetWindowLongPtr(gameWindow, GWLP_WNDPROC, (LONG_PTR)NewWndProc);
	ImGui_ImplWin32_Init(gameWindow);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.Fonts->AddFontDefault();
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	
	ImGui::SetNextWindowSize(windowSize);
	initialized = true;
	std::cout << "Initialized" << std::endl;
}
void Menu::StartRender()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}



float RelativePosX = 0.0f;
float RelativePosY = 0.0f;
float RelativePosZ = 0.0f;

void Menu::Render()
{
	if (!showMenu)
		return;

	oSetRelativeMouseMode(!showMenu);
	ImGui::Begin("Flow", &showMenu);
	ImGui::Text("Features");
	//if (ImGui::Button("Aimbot")) {
	//	playerPtr->Position.y += 10.0f;
	//}
	ImGui::Checkbox("Aimbot", &Settings::Aimbot::enabled);
	ImGui::Checkbox("ESP", &Settings::ESP::enabled);
	ImGui::Text("Teleport");
	ImGui::Checkbox("Show Teams", &Settings::ESP::drawTeams);


	ImGui::Text("Teleport");
	ImGui::SliderFloat("X", &RelativePosX, -100.0f, 100.0f);
	ImGui::SliderFloat("Y", &RelativePosY, -100.0f, 100.0f);
	ImGui::SliderFloat("Z", &RelativePosZ, -100.0f, 100.0f);
	if (ImGui::Button("Teleport")) {
		if (playerPtr) {
			playerPtr->Position.x += RelativePosX;
			playerPtr->Position.y += RelativePosZ;
			playerPtr->Position.z += RelativePosY;
		}
	}

	ImGui::End();

}
void SetupContext(HDC& hdc) {
	myContext = wglCreateContext(hdc);

	wglMakeCurrent(hdc, myContext);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	contextCreated = true;
}
void Menu::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}
BOOL __stdcall Menu::newSwapBuffers(HDC hdc)
{
	if (!initialized) {
		Init();
		return oSwapBuffers(hdc);
	}

	gameContext = wglGetCurrentContext();
	if (!contextCreated) {
		SetupContext(hdc);
	}
	wglMakeCurrent(hdc, myContext);

	Menu::StartRender();

	esp::esp();

	Menu::Render();
	Menu::EndRender();

	wglMakeCurrent(hdc, gameContext);
	return oSwapBuffers(hdc);
}
