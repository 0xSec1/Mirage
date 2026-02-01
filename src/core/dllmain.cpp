#include <stdio.h>
#include <windows.h>
#include "HookEngine.hpp"

void SetupConsole(){
    if(AllocConsole()){
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        SetConsoleTitleA("Test window");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            // SetupConsole();

            if(!HookEngine::Initialize()){
                printf("[!] Hook engine failed to start\n");
            }
            break;

        case DLL_PROCESS_DETACH:
            HookEngine::Shutdown();
            break;
    }
    return TRUE;
}
