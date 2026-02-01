#include <windows.h>
#include <iostream>

int main(){
    std::cout << "\n[ACTION] Please attach your debugger NOW." << std::endl;
    std::cout << "[ACTION] Press ENTER once attached to continue..." << std::endl;
    std::cin.get();
    std::cout << "[TARGET] Calling IsDebuggerPresent()..." << std::endl;

    BOOL isDebugged = IsDebuggerPresent();
    if(isDebugged){
        std::cout << "[TARGET] RESULT: TRUE (Debugger detected)" << std::endl;
        std::cout << "[TARGET] STATUS: FAIL (I am watching YOU)" << std::endl;
    }
    else{
        std::cout << "[TARGET] RESULT: FALSE (No Debugger Found)" << std::endl;
        std::cout << "[TARGET] STATUS: SUCCESS (I think I am safe)" << std::endl;
    }

    std::cout << "\nPress enter to exit..";
    std::cin.get();
    return 0;
}
