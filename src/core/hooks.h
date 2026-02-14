#pragma once
#include "common.h"
#include "nt_defs.h"

#define DECLARE_HOOK(RType, Name, ...)\
    typedef RType(NTAPI *P##Name)(__VA_ARGS__);\
    inline P##Name fp##Name = nullptr;\
    RType NTAPI Detour##Name(__VA_ARGS__);

DECLARE_HOOK(BOOL, IsDebuggerPresent, VOID);
DECLARE_HOOK(BOOL, CheckRemoteDebuggerPresent, HANDLE hProcess, PBOOL pbDebuggerPresent);
DECLARE_HOOK(HANDLE, CreateToolhelp32Snapshot, DWORD dwFlags, DWORD th32ProcessID);
DECLARE_HOOK(BOOL, Process32FirstW, HANDLE hSnapshot, LPPROCESSENTRY32W lppe);
DECLARE_HOOK(BOOL, Process32NextW, HANDLE hSnapshot, LPPROCESSENTRY32W lppe);

DECLARE_HOOK(NTSTATUS, NtQuerySystemInformation,
        SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength
);
