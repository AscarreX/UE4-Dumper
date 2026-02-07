#pragma once

#include "Memory.h"

struct FString {
    uintptr_t Data;
    INT32 Count;

    const char* c_str() {
        char* str = new char[Count];
        for (int i = 0; i < Count; i++) {
            char data = Read<char>(Data + (2 * i));
            str[i] = isascii(data) ? data : '?';
        }
        return (const char*)str;
    }

    std::string toString() const {
        std::string str;
        str.reserve(Count);
        for (int i = 0; i < Count; i++) {
            char data = Read<char>(Data + (2 * i));
            str += isascii(data) ? data : '?';
        }
        return str;
    }
};

struct WideStr {
    static bool is_surrogate(uint16_t uc) {
        return (uc - 0xd800u) < 2048u;
    }

    static bool is_high_surrogate(uint16_t uc) {
        return (uc & 0xfffffc00) == 0xd800;
    }

    static bool is_low_surrogate(uint16_t uc) {
        return (uc & 0xfffffc00) == 0xdc00;
    }

    static wchar_t surrogate_to_utf32(uint16_t high, uint16_t low) {
        return (high << 10) + low - 0x35fdc00;
    }

    static std::wstring w_str(uintptr_t str, size_t len) {
        std::vector<uint16_t> source = ReadArray<uint16_t>(str, len);
        std::wstring output(len, L'\0');

        for (size_t i = 0; i < len; i++) {
            const uint16_t uc = source[i];
            if (!is_surrogate(uc)) {
                output[i] = uc;
            } else {
                if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                    output[i] = surrogate_to_utf32(uc, source[i]);
                else
                    output[i] = L'?';
            }
        }

        return output;
    }

    static std::string getString(uintptr_t StrPtr, int StrLength) {
        std::wstring str = w_str(StrPtr, StrLength);

        size_t bufferSize = str.size() * 4 + 1;
        std::vector<char> buffer(bufferSize, '\0');
        size_t converted = 0;

        errno_t err = wcstombs_s(&converted, buffer.data(), bufferSize, str.c_str(), _TRUNCATE);
        if (err != 0) return "";
        return std::string(buffer.data());
    }
};

std::string GetNameFromFName(int index) {
    if (Offsets::isUE423) {
        uint32_t Block = index >> 16;
        uint16_t Offset = index & 65535;

        uintptr_t FNamePool = (Memory.ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool;

        uintptr_t NamePoolChunk = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (Block * Offsets::PointerSize));
        uintptr_t FNameEntry = NamePoolChunk + (Offsets::FNameStride * Offset);

        INT16 FNameEntryHeader = Read<INT16>(FNameEntry + Offsets::FNameEntryHeader);
        uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;

        bool wide = FNameEntryHeader & 1;

        if (StrLength > 0 && StrLength < 250) {
            if (Offsets::isXorDecrypt) {
                return wide ? DecryptXorCypher(WideStr::getString(StrPtr, StrLength)) : DecryptXorCypher(ReadStringNew(StrPtr, StrLength));
            } else {
                return wide ? WideStr::getString(StrPtr, StrLength) : ReadStringNew(StrPtr, StrLength);
            }
        } else {
            return "None";
        }
    } else {
        static uintptr_t TNameEntryArray;

        if (Offsets::isDereferencing) {
            TNameEntryArray = Read<uintptr_t>(Read<uintptr_t>(Memory.ModuleBase + Offsets::GNames));
        } else {
            TNameEntryArray = Read<uintptr_t>(Memory.ModuleBase + Offsets::GNames);
        }

        uintptr_t FNameEntryArray = Read<uintptr_t>(TNameEntryArray + ((index / 0x4000) * Offsets::PointerSize));
        uintptr_t FNameEntry = Read<uintptr_t>(FNameEntryArray + ((index % 0x4000) * Offsets::PointerSize));

        if (Offsets::isXorDecrypt) {
            return DecryptXorCypher(ReadString(FNameEntry + Offsets::TNameEntryToNameString));
        } else {
            return ReadString(FNameEntry + Offsets::TNameEntryToNameString);
        }
    }
}
