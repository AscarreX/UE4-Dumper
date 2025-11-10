#include "Dumper.h"

static std::string GetExeDir() {
	char path[MAX_PATH];
	if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
		return ".";
	}
	std::string exePath(path);
	return exePath.substr(0, exePath.find_last_of("\\/"));
}

void RunKeyLoop(const std::string& baseDir) {
	std::vector<KeyAction> actions = {
		{ VK_F1,  "Dump String", [&] { DumpStrings(baseDir); } },
		{ VK_F2,  "Objects Dump", [&] { DumpObjects(baseDir); } },
		{ VK_F3,  "SDK Dump (GUObject)", [&] { DumpSDK(baseDir); } },
		{ VK_F4,  "SDKW Dump (GWorld)", [&] { DumpSDKW(baseDir); } },
		{ VK_F5,  "Dump Actors", [&] { DumpActors(baseDir); } },
		{ VK_F6,  "Dump Bones", [&] { BonesDump(baseDir); } },
		{ VK_END, "Exit", [&] {
			std::cout << "Exiting program...\n";
			CloseHandle(Memory.ProcessHandle);
			PostQuitMessage(0);
			exit(0);
		}}
	};

	while (IsProcessRunning()) {
		for (const auto& action : actions) {
			if (GetAsyncKeyState(action.key) & 1) {
				action.action();
			}
		}
		Sleep(50); // avoid pegging the CPU
	}
}

int main() {
	// Check for Unreal Engine window
	std::cout << "[*] Checking for Unreal Engine window..." << std::endl;

	HWND unrealWindow = FindWindowW(L"UnrealWindow", nullptr);
	if (!unrealWindow) {
		std::cout << "[!] No Unreal Engine window found!" << std::endl;
		std::cout << "[!] Please make sure the game is running." << std::endl;
		system("pause");
		return 1;
	}

	std::cout << "[+] Unreal Engine window detected" << std::endl;

	// Proceed with hardcoded offsets
	Offsets::Example1(); // Load only required offsets in these functions

	// Basic validation
	Memory.ProcessId = GetProcessID();
	if (Memory.ProcessId == 0) {
		std::cerr << "[!] Failed to get Process ID" << std::endl;
		return 1;
	}

	Memory.ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, Memory.ProcessId);
	if (!Memory.ProcessHandle || Memory.ProcessHandle == INVALID_HANDLE_VALUE) {
		std::cerr << "[!] Failed to get Process Handle" << std::endl;
		return 1;
	}

	Memory.ModuleBase = GetBaseAddress();
	if (Memory.ModuleBase == 0) {
		std::cerr << "[!] Failed to get Module Base" << std::endl;
		return 1;
	}

	std::cout << "[*] Found ProcessId at " << Memory.ProcessId << std::endl;
	std::cout << "[*] Obtained Process Handle " << std::endl;
	std::cout << "[*] Found Module Base at 0x" << std::hex << Memory.ModuleBase << std::dec << std::endl;

	std::string baseDir = GetExeDir() + "\\Dumped\\" + Memory.GameName;
	if (!fs::exists(baseDir)) {
		if (!fs::create_directories(baseDir)) {
			std::cerr << "[!] Failed to create directory: " << baseDir << std::endl;
		}
	}

	std::cout << "[*] Create directory in: " << baseDir << std::endl;

	std::cout << "\n=== Dumper Menu ===" << std::endl;
	std::cout << "1. Dump String - F1" << std::endl;
	std::cout << "2. Objects Dump - F2" << std::endl;
	std::cout << "3. SDK Dump (GUObject) - F3" << std::endl;
	std::cout << "4. SDKW Dump (GWorld) - F4" << std::endl;
	std::cout << "5. Dump Actors - F5" << std::endl;
	std::cout << "6. Dump Bones - F6" << std::endl;
	std::cout << "7. Exit - End" << std::endl;

	RunKeyLoop(baseDir);

	return 0;
}