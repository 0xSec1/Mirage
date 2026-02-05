## Mirage - Anti-Anti-Debug Framework for Malware
Mirage is a modular dynamic binary instrumentation framework designed to defeat anti-debugging techniques employed by malware. Unlike traditional debuggers that rely solely on API hooking, Mirage implements a multi-layered evasion strategy involving process injection, API interception (MinHook), and direct memory manipulation (PEB patching). The tool allows researchers to attach standard debuggers (e.g. x64dbg) to hostile applications without triggering anti-debug mechanisms such as IsDebuggerPresent, CheckRemoteDebuggerPresent, or direct PEB flag checks.

## Feature
* **Suspended Process Injection**: Launches malware in a suspended state to ensure early-stage instrumentation before the entry point executes.
* **API Hooking**: Intercepts standard kernel32.dll export calls to falsify debugger presence.
* **PEB Sanitization** : Directly modifies the Process Environment Block (PEB) to clear BeingDebugged and NtGlobalFlag indicators.
* **Process Hiding** Intercepts process enumeration APIs to mask the presence of known debugging tools from the malware.

## Build Instruction
**Prerequisites**: CMake(3.15+), Visual Studio 2019/2022

```powershell
git clone https://github.com/0xSec1/Mirage.git
cd Mirage

cmake -S. -B build
cmake --build build --config Release

```
## Usage
> [!NOTE]
> Test app is there to test implemented functionalities named `victim.exe`

1. Locate binary in `build/bin/Release`
2. Run loader with target executable
```powershell
.\MirageLoader.exe <path_to_malware.exe>

```
## Disclaimer
It is developed for educational and research purposes only.
