#include "hooks.h"

const std::vector<std::wstring> tools = {
    L"x64dbg.exe",
    L"x32dbg.exe",
    L"ollydbg.exe"
};

bool IsHidden(const WCHAR* processName){
    std::wstring currentProc(processName);
    std::transform(currentProc.begin(), currentProc.end(), currentProc.begin(), ::tolower);

    for(const auto& tool : tools){
        if(currentProc == tool){
            return true;
        }
    }

    return false;
}

BOOL NTAPI DetourIsDebuggerPresent(VOID){
    log("Intercepted IsDebuggerPresent(). Returning false");
    return FALSE;
}

BOOL NTAPI DetourCheckRemoteDebuggerPresent(HANDLE hProcess, PBOOL pbDebuggerPresent){
    if(pbDebuggerPresent){
        *pbDebuggerPresent = FALSE;
        log("Intercepted CheckRemoteDebuggerPresent. Setting pbDebuggerPresent to FALSE");
    }
    return TRUE;
}

HANDLE WINAPI DetourCreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID){
    log("Intercepted CreateToolhelp32Snapshot");                //only log, if tried to block it malware will crash
    return fpCreateToolhelp32Snapshot(dwFlags, th32ProcessID);
}

//main hook will be on Process32First and Process32Next

BOOL WINAPI DetourProcess32FirstW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe){
    BOOL result = fpProcess32FirstW(hSnapshot, lppe);           //get first process

    while(result && IsHidden(lppe->szExeFile)){                 //loop only possible if tools PID 0
        log("Process filtered from Process32FirstW: %ls", lppe->szExeFile);
        result = fpProcess32NextW(hSnapshot, lppe);             //skips to next process
    }

    return result;
}

BOOL WINAPI DetourProcess32NextW(HANDLE hSnapshot, LPPROCESSENTRY32W lppe){
    BOOL result = fpProcess32NextW(hSnapshot, lppe);
    while(result && IsHidden(lppe->szExeFile)){
        log("Spoofing Process List: Hiding %ls", lppe->szExeFile);
        result = fpProcess32NextW(hSnapshot, lppe);
    }

    return result;
}
