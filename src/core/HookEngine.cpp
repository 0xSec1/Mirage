#include "HookEngine.hpp"
#include "hooks.h"
#include "../../external/minhook/include/MinHook.h"

#define INSTALL_HOOK(DLL, Name)\
    InstallHook(DLL, #Name, (LPVOID)Detour##Name, (LPVOID*)&fp##Name)

bool HookEngine::Initialize(){
    log("Initlizing MinHook Engine...");

    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WatcherThread, NULL, 0, NULL);

    if(hThread){
        log("[MIRAGE] Watcher thread created");
        CloseHandle(hThread);
    }
    else{
        error("[MIRAGE] Failed to create watcher thread");
    }

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

void HookEngine::SanitizePEB(){
    #ifdef _WIN64
        PPEB pPeb = (PPEB)__readgsqword(0x60);
    #else
        PPEB pPeb = (PPEB)__readfsdword(0x30);
    #endif

        if(!pPeb){
            error("[CRITICAL]: Failed to locate PEB address");
            return;
        }

        //Read beingDebugged at offset 0x2
        int beingDebugged = (int)pPeb->BeingDebugged;
        if(beingDebugged == 1){
            log("[PEB] beingDebugged flag: 1 [Detected]. Patching it...");
            pPeb->BeingDebugged = 0;    //patch it
        }

    //Read NtGlobalFlag
    #ifdef _WIN64
        DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0xBC);
    #else
        DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0x68);
    #endif

        if(dwNtGlobalFlag != 0){
            log("[PEB] NtGlobalFlag: [0x%X] Detected. Patching it...", dwNtGlobalFlag);
            dwNtGlobalFlag = 0;
        }
}

//watches PEB
DWORD WINAPI HookEngine::WatcherThread(LPVOID lpParam){
    log("[THREAD] Watcher thread starts. Polling PEB");
    while(true){
        SanitizePEB();
        Sleep(100);
    }

    return 0;
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
