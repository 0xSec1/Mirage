#include <windows.h>
#include <iostream>

void PauseForDebugger(){
    std::cout << "\n[ACTION] Please attach your debugger NOW." << std::endl;
    std::cout << "[ACTION] Press ENTER once attached to continue..." << std::endl;
    std::cin.get();
}

int main(){
    PauseForDebugger();
    std::cout << "[TARGET 1] Calling IsDebuggerPresent()..." << std::endl;

    BOOL isDebugged = IsDebuggerPresent();
    if(isDebugged){
        std::cout << "FAIL -> Debugger Found" << std::endl;
    }
    else{
        std::cout << "PASS -> No Debugger Found" << std::endl;
    }

    //CheckRemoteDebugger Test
    std::cout << "\n[TARGET 2] Calling CheckRemoteDebuggerPresent()..." << std::endl;

    BOOL isRemoteDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);

    if(isRemoteDebuggerPresent){
        std::cout << "FAIL -> Debugger Found" << std::endl;
    }
    else{
        std::cout << "PASS -> No Debugger Found" << std::endl;
    }

    std::cout << "\nPress enter to exit..";
    std::cin.get();
    return 0;
}
