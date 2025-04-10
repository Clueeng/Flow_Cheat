#pragma once
#include <Windows.h>
#include "genCode.h"

extern HMODULE hModule;
extern uintptr_t exeBaseAddress;

extern PlayerEntity* playerPtr;
extern EntityList* players;
extern uintptr_t* playerCount;

void resetPointers();


typedef BOOL(__stdcall* _wglSwapBuffers)(HDC hdc);
extern _wglSwapBuffers oSwapBuffers;

typedef int(__fastcall* _SDL_SetRelativeMouseMode)(int mode);
extern _SDL_SetRelativeMouseMode oSetRelativeMouseMode;