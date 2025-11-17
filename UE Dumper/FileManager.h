#pragma once


namespace fs = std::filesystem;

class FileSystemManager {
private:
    inline static std::string baseDir;

    FileSystemManager() = default;

    static std::string GetExeDir() {
        char path[MAX_PATH];
        if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
            return ".";
        }
        std::string exePath(path);
        return exePath.substr(0, exePath.find_last_of("\\/"));
    }

public:
    static void Init(const std::string& gameName) {
        std::string exeDir = GetExeDir();
        baseDir = exeDir + "\\Dumped\\" + gameName;
        CreateDirectory(baseDir);
        //CreateDirectory(baseDir + "\\SDK");
    }

    static void CreateDirectory(const std::string& path) {
        if (!fs::exists(path)) {
            if (!fs::create_directories(path)) {
                std::cerr << "[!] Failed to create directory: " << path << std::endl;
            }
        }
    }

    static void DumpFile(const std::string& filename, const std::string& data) {
        std::ofstream file(baseDir + "\\" + filename, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "[!] Failed to open file: " << baseDir + "\\" + filename << std::endl;
            return;
        }
        file << data;
        file.close();
    }

    static void AppendToFile(const std::string& filename, const std::string& data, bool reset = false) {
        std::string fullPath = baseDir + "\\" + filename;

        if (reset && fs::exists(fullPath)) {
            fs::remove(fullPath);
        }

        std::ofstream file(fullPath, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            std::cerr << "[!] Failed to open file: " << fullPath << std::endl;
            return;
        }

        file << data;
        file.close();
    }
};