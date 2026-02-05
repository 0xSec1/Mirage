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

void ScanProcess(){
    std::cout << "\n[TARGET] Scanning running processes..." << std::endl;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(hSnap == INVALID_HANDLE_VALUE){
        return;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    bool foundTool = false;

    if(Process32FirstW(hSnap, &pe32)){
        do{
            std::wstring name = pe32.szExeFile;
            if(name == L"x64dbg.exe" || name == L"x32dbg.exe" || name == L"ollydbg.exe"){
                std::wcout << "[ALERT] Found Debugger Tool: " << name.c_str() << std::endl;
                foundTool = true;
            }
        } while(Process32NextW(hSnap, &pe32));
    }

    CloseHandle(hSnap);

    if(!foundTool){
        std::cout << "[TARGET] Process Scan Complete. Nothing Found" << std::endl;
    }
}

int main(){
    PauseForDebugger();

    //Scan Process
    ScanProcess();

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
