#pragma once
#include <windows.h>
#include <winternl.h>
#include <intrin.h> // __readgsqword or __readfsdword
#include <cstdio>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <algorithm>

#define log(fmt, ...) printf("[MIRAGE] " fmt "\n", ##__VA_ARGS__)
#define error(fmt, ...) printf("[!] ERROR: " fmt "\n", ##__VA_ARGS__)

#ifndef NTAPI
#define NTAPI __stdcall
#endif
