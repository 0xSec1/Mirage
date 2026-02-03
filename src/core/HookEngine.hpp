#pragma once
#include "common.h"

class HookEngine{
    public:
        static bool Initialize();
        static void Shutdown();
        static void SanitizePEB();
    private:
        static DWORD WINAPI WatcherThread(LPVOID lpParam);
        static bool InstallHook(LPCWSTR moduleName, LPCSTR procName, LPVOID detourFunc, LPVOID* originalFunc);
};
