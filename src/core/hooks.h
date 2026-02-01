#pragma once
#include "common.h"

#define DECLARE_HOOK(RType, Name, ...)\
    typedef RType(NTAPI *P##Name)(__VA_ARGS__);\
    inline P##Name fp##Name = nullptr;\
    RType NTAPI Detour##Name(__VA_ARGS__);

DECLARE_HOOK(BOOL, IsDebuggerPresent, VOID);
