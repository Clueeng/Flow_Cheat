#pragma once
#include <Windows.h>

namespace Menu {
	void ToggleMenu();
	void Init();
	void StartRender();
	void Render();
	void EndRender();
	BOOL __stdcall newSwapBuffers(HDC hdc);
}