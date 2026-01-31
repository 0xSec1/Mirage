#include <system_error>
#include <filesystem>
#include <windows.h>
#include <iostream>
#include <string>

//prints error and exits
void error(const std::string& msg){
    DWORD errorNo = GetLastError();
    std::cerr << "FATAL: " << msg << " | Error Code: " << errorNo << " (" << std::system_category().message(errorNo) << ")" << std::endl;
    ExitProcess(1);
}

void log(const std::string& msg){
    std::cout << msg << std::endl;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        log("Usage: MirageLoader.exe <path_to_executable>");
        return 1;
    }

    std::string targetPath = argv[1];
    std::string dllName = "MirageCore.dll";

    //gets full path of dll
    char currentDir[MAX_PATH];
    if(GetModuleFileNameA(NULL, currentDir, MAX_PATH) == 0){
        error("Failed to get loader path");
    }

    std::filesystem::path loaderPath(currentDir);
    std::filesystem::path dllPath = loaderPath.parent_path() / dllName;

    if(!std::filesystem::exists(dllPath)){
        error("Could not find " + dllName + " in the loader directory: " + dllPath.string());
    }

    log("Target: " + targetPath);
    log("DLL Path: " + dllPath.string());

    std::string dPath = dllPath.string();

    //launch target in suspended state
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    if(!CreateProcessA(targetPath.c_str(), NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)){
        error("Failed to create process.");
    }

    log("Process launched in SUSPENDED state PID: " + std::to_string(pi.dwProcessId));

    //allocate memory in target process
    void* pRemotePath = VirtualAllocEx(pi.hProcess, NULL, dPath.size() + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if(!pRemotePath){
        error("Failed to allocate memory in process");
    }

    //write path to allocated memory
    if(!WriteProcessMemory(pi.hProcess, pRemotePath, dPath.c_str(), dPath.size() + 1, NULL)){
        error("Failed to write to memory of process");
    }

    //load and execute
    HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), pRemotePath, 0, NULL);

    if(!hThread){
        error("Failed to inject dll");
    }

    log("Injected thread created");

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    //resume main thread
    log("Dll injection success. Resume main thread..");
    ResumeThread(pi.hThread);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
