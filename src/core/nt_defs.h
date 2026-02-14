#pragma once
#include "common.h"

// 0x23 -> 35
#define SystemKernelDebuggerInformation 35

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION{
    BOOLEAN KernelDebuggerEnabled;
    BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef NTSTATUS (NTAPI *P_NtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);
