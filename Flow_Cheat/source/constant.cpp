#include "constant.h"

HMODULE hModule = GetModuleHandle(nullptr);
uintptr_t exeBaseAddress = (uintptr_t)GetModuleHandle(nullptr);



extern PlayerEntity* playerPtr = *(PlayerEntity**)(exeBaseAddress + 0x0017E0A8);

uintptr_t playersAddr = (exeBaseAddress + 0x18AC04);
extern EntityList* players = *(EntityList**)playersAddr;
extern uintptr_t* playerCount = (uintptr_t*)(exeBaseAddress + 0x191FD4);
extern float* viewMatrix = (float*)(exeBaseAddress + 0x17DFD0); // 0x17DFE0 0x17DFD0

void resetPointers() {
	playerPtr = *(PlayerEntity**)(exeBaseAddress + 0x0017E0A8);
	players = *(EntityList**)playersAddr;
	playerCount = (uintptr_t*)(exeBaseAddress + 0x191FD4);

}
_wglSwapBuffers oSwapBuffers = (_wglSwapBuffers)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
_SDL_SetRelativeMouseMode oSetRelativeMouseMode = (_SDL_SetRelativeMouseMode)GetProcAddress(GetModuleHandle(L"SDL2.dll"), "SDL_SetRelativeMouseMode");

