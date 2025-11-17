#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <tchar.h>
#include <Psapi.h>
#include <string>
#include <thread>
#include <vector>
#include <list>
#include <map>

#include <unordered_set>

namespace fs = std::filesystem;

std::vector<UINT32> structIDMap;

struct KeyAction {
    int key;
    std::string name;
    std::function<void()> action;
};

struct MemoryRequirements {
    LPCSTR TargetProcess;
    LPCSTR GameName;
    DWORD ProcessId;
    HANDLE ProcessHandle;
    uintptr_t ModuleBase;
} Memory;
