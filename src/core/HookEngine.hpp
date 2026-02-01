#pragma once
#include "common.h"

class HookEngine{
    public:
        static bool Initialize();
        static void Shutdown();
    private:
        static bool InstallHook(LPCWSTR moduleName, LPCSTR procName, LPVOID detourFunc, LPVOID* originalFunc);
};
