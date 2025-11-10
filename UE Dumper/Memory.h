#pragma once

#include "Offsets.h"

int GetProcessID(LPCSTR TargetProcess = Memory.TargetProcess) {
	int pid = 0;
	int threadCount = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);
	while (Process32Next(hSnap, &pe)) {
		if (_tcsicmp(pe.szExeFile, _T(TargetProcess)) == 0) {
			if ((int)pe.cntThreads > threadCount) {
				threadCount = pe.cntThreads;
				pid = pe.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnap);
	return pid;
}

uintptr_t GetBaseAddress(LPCSTR TargetProcess = Memory.TargetProcess, DWORD ProcessId = Memory.ProcessId) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!strcmp(modEntry.szModule, TargetProcess)) {
					CloseHandle(hSnap);
					return (uintptr_t)modEntry.modBaseAddr;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
}

bool IsProcessRunning(DWORD ProcessId = Memory.ProcessId) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ProcessId);
	if (hProcess == NULL) {
		return false;
	}

	DWORD exitCode;
	if (GetExitCodeProcess(hProcess, &exitCode)) {
		CloseHandle(hProcess);
		return exitCode == STILL_ACTIVE;
	}
	else {
		CloseHandle(hProcess);
		return false;
	}
}

template<typename T> T Read(SIZE_T address, size_t size = sizeof(T), HANDLE ProcessHandle = Memory.ProcessHandle) {
	T buffer;
	ReadProcessMemory(ProcessHandle, (LPCVOID)address, &buffer, size, NULL);
	return buffer;
}

template <typename T>
constexpr void Write(const uintptr_t& address, const T& value, size_t size = sizeof(T), HANDLE ProcessHandle = Memory.ProcessHandle) {
	WriteProcessMemory(ProcessHandle, reinterpret_cast<void*>(address), &value, size, NULL);
}

template<typename T>
std::vector<T> ReadArray(uintptr_t address, size_t size) {
	std::vector<T> data(size);
	for (int i = 0; i < size; i++) {
		data[i] = Read<T>(address + i * sizeof(T));
	}
	return data;
}

std::string ReadString(uintptr_t address, size_t  size = 100) {
	std::vector<char> data(size, '\0');

	for (int i = 0; i < size; i++) {
		data[i] = Read<char>(address + i);
		if (data[i] == 0x0) {
			break;
		}
	}

	std::string name(data.data());
	name.shrink_to_fit();
	return name;
}

std::string ReadStringNew(uintptr_t address, size_t  size = 100) {
	std::string name(size, '\0');
	for (unsigned int i = 0; i < size; i++) {
		name[i] = Read<char>(address + i);
	}
	name.shrink_to_fit();
	return name;
}

std::string DecryptXorCypher(std::string Input, int size = 0) {
	if (size == 0) size = Input.size();
	std::string Output = Input;

	for (int i = 0; i < size; i++)
		Output[i] = Input[i] ^ size;

	return Output;
}
