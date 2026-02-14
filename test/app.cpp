#include <iostream>
#include "../src/core/nt_defs.h"

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
                std::wcout << "  [ALERT] Found Debugger Tool: " << name.c_str() << std::endl;
                foundTool = true;
            }
        } while(Process32NextW(hSnap, &pe32));
    }

    CloseHandle(hSnap);

    if(!foundTool){
        std::cout << "[TARGET] Process Scan Complete. Nothing Found" << std::endl;
    }
}

void CheckKernelDebugger(){
    std::cout << "\n[TARGET] Checking if kernel debugger present..." << std::endl;

    HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
    auto pNtQuerySystemInformation = (P_NtQuerySystemInformation)GetProcAddress(hNtdll, "NtQuerySystemInformation");

    if(!pNtQuerySystemInformation){
        std::cout << "  [ERROR] Could not resolve NtQuerySystemInformation" << std::endl;
        return;
    }

    SYSTEM_KERNEL_DEBUGGER_INFORMATION info = {0};
    NTSTATUS status = pNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemKernelDebuggerInformation, &info, sizeof(info), NULL);

    if(status == 0){
        std::cout  << "  [INFO] KernelDebuggerEnabled: " << (int)info.KernelDebuggerEnabled << std::endl;
        std::cout  << "  [INFO] KernelDebuggerNotPresent: " << (int)info.KernelDebuggerNotPresent << std::endl;

        if(info.KernelDebuggerEnabled || !info.KernelDebuggerNotPresent){
            std::cout << "  FAIL -> Kernel Debugger detected" << std::endl;
        }
        else{
            std::cout << "  PASS -> No Kernel Debugger present" << std::endl;
        }
    }
    else{
        std::cout << "  [ERROR] API Call failed" << std::endl;
    }
}

int main(){
    PauseForDebugger();

    //Kernel Debugger
    CheckKernelDebugger();
    //Scan Process
    ScanProcess();

    //checks PEB
    if(readPEB()){
        std::cout << "\n[TARGET] Debugger detected via PEB" <<  std::endl;
        std::cout << "[TARGET] Terminating process to evade analysis" << std::endl;
        ExitProcess(1);
    }

    std::cout << "\n[TARGET 1] Calling IsDebuggerPresent()..." << std::endl;

    BOOL isDebugged = IsDebuggerPresent();
    if(isDebugged){
        std::cout << "  FAIL -> Debugger Found" << std::endl;
    }
    else{
        std::cout << "  PASS -> No Debugger Found" << std::endl;
    }

    //CheckRemoteDebugger Test
    std::cout << "\n[TARGET 2] Calling CheckRemoteDebuggerPresent()..." << std::endl;

    BOOL isRemoteDebuggerPresent = FALSE;
    CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);

    if(isRemoteDebuggerPresent){
        std::cout << "  FAIL -> Debugger Found" << std::endl;
    }
    else{
        std::cout << "  PASS -> No Debugger Found" << std::endl;
    }

    std::cout << "\nPress enter to exit..";
    std::cin.get();
    return 0;
}
