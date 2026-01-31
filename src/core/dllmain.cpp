#include <stdio.h>
#include <windows.h>

void SetupConsole(){
    if(AllocConsole()){
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONOUT$", "w", stderr);
        SetConsoleTitleA("F*ck Windows");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            SetupConsole();
            break;
    }
    return TRUE;
}
