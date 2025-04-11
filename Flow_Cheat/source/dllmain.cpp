// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include <iostream>
#include <fstream>
#include <Windows.h>
#include "constant.h"
#include "genCode.h"
#include <sstream>
#include <string>
#include "esp.h"
#include "detours/detours.h"
#include "imgui.h"
#include "menu.h"


template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
std::ofstream logFile;

void logPrint(const std::string& message)
{
    std::cout << message;
    if (logFile.is_open())
    {
        logFile << message;
        logFile.flush();
    }
}

void aimbot() {
    while (true) {
        resetPointers();
        esp::aimbot();
        Sleep(50);
        if(GetAsyncKeyState(VK_DELETE) & 1)
			Menu::ToggleMenu();
    }
}

void hook() {
	DisableThreadLibraryCalls(hModule);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oSwapBuffers, Menu::newSwapBuffers);
    DetourTransactionCommit();

    while (true) {
        esp::esp();
        Sleep(33);
    }
}

bool fly = FALSE;

void console() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);


    std::cout << ("Console created\n");

    logFile.open("C:\\Users\\flaily\\AppData\\Roaming\\Flow\\output.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cout << "Failed to open log file!" << std::endl;
        std::cout << ("Failed to open log file!\n");
    }
    else {
        logPrint("Log file successfully opened.\n");
        std::cout << "Opened file at " << "C:\\Users\\flaily\\AppData\\Roaming\\Flow\\output.txt";
    }

    while (true) {
        std::cout << ("> ");
        std::string input;
        std::cin >> input; 
        std::cout << ("\n");

        if (input == "amount") {
            std::cout << (to_string_custom(*playerCount) + "\n");
        }

        if (input == "list" || (GetAsyncKeyState(VK_F6) & 0x8000)) {
            for (int i = 0; i <= *playerCount; i++) {
                if (players->players[i] == nullptr || players->players[i]->vTable == nullptr) {
                    continue;
                }
                std::cout << "Entity " << i << "Team " << players->players[i]->Team << ": " << "Pos " << 
                    players->players[i]->Position.x << " " << 
                    players->players[i]->Position.y << " " << 
                    players->players[i]->Position.z << ". ";
				std::cout << "Health: " << players->players[i]->Health << std::endl;
            } 
        }
        if (input == "player") {
            PlayerEntity* player = playerPtr;
            logPrint("Player " + std::string(player->Name) + ": " +
                to_string_custom(player->Position.x) + ", " +
                to_string_custom(player->Position.y) + ", " +
                to_string_custom(player->Position.z) + "\n" +
                "Health: " + to_string_custom(player->Health) + "\n");
        }
    }

    logFile.close();
    FreeConsole();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hook, nullptr, 0, nullptr);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)aimbot, nullptr, 0, nullptr);
        break; 
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
