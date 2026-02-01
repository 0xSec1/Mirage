#include "hooks.h"

BOOL NTAPI DetourIsDebuggerPresent(VOID){
    log("Intercepted IsDebuggerPresent(). Returning false");
    return FALSE;
}
