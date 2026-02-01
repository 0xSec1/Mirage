#include "hooks.h"

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
