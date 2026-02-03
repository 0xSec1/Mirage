#include <iostream>
#include "../src/core/common.h"

void PauseForDebugger(){
    std::cout << "\n[ACTION] Please attach your debugger NOW." << std::endl;
    std::cout << "[ACTION] Press ENTER once attached to continue..." << std::endl;
    std::cin.get();
}

bool readPEB(){
#ifdef _WIN64
    PPEB pPeb = (PPEB)__readgsqword(0x60);
#else
    PPEB pPeb = (PPEB)__readfsdqord(0x30);
#endif
    return (pPeb -> BeingDebugged == 1);
}

int main(){
    PauseForDebugger();

    //checks PEB
    if(readPEB()){
        std::cout << "[TARGET] Debugger detected via PEB" <<  std::endl;
        std::cout << "[TARGET] Terminating process to evade analysis" << std::endl;
        ExitProcess(1);
    }

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
