#include "HookEngine.hpp"
#include "hooks.h"
#include <MinHook.h>

#define INSTALL_HOOK(DLL, Name)\
    InstallHook(DLL, #Name, (LPVOID)Detour##Name, (LPVOID*)&fp##Name)

bool HookEngine::Initialize(){
    log("Initlizing MinHook Engine...");

    if(MH_Initialize() != MH_OK){
        error("MinHook initilization failed!");
        return false;
    }

    INSTALL_HOOK(L"kernel32.dll", IsDebuggerPresent);
    INSTALL_HOOK(L"kernel32.dll", CheckRemoteDebuggerPresent);
    log("All hooks installed");
    return true;
}

void HookEngine::Shutdown(){
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    log("Hook Engine Shutdown");
}

bool HookEngine::InstallHook(LPCWSTR moduleName, LPCSTR procName, LPVOID detourFunc, LPVOID* originalFunc){

    HMODULE hModule = GetModuleHandleW(moduleName);
    if(!hModule){
        hModule = LoadLibraryW(moduleName);
        return false;
    }
    else if(!hModule){
        error("Module not found: %ls", moduleName);
        return false;
    }

    LPVOID targetFunc = (LPVOID)GetProcAddress(hModule, procName);
    if(!targetFunc){
        error("Function not found: %s", procName);
        return false;
    }

    MH_STATUS createStatus = MH_CreateHook(targetFunc, detourFunc, originalFunc);
    if(createStatus != MH_OK){
        error("MH_CreateHook failed for %s: %s", procName, MH_StatusToString(createStatus));
        return false;
    }

    MH_STATUS enableStatus = MH_EnableHook(targetFunc);
    if(enableStatus != MH_OK){
        error("MH_EnableHook failed for %s: %s", procName, MH_StatusToString(enableStatus));
        return false;
    }

    log("Hooked: %ls -> %s", moduleName, procName);
    return true;
}
