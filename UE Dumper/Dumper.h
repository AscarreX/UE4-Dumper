#pragma once

#include "PropertyFlags.h"

void DumpBlocks(uintptr_t FNamePool, UINT32 blockId, UINT32 blockSize, int& count, std::ofstream& Dump) {
    uintptr_t It = Read<uintptr_t>(FNamePool + Offsets::FNamePoolToBlocks + (blockId * Offsets::PointerSize));
    uintptr_t End = It + blockSize - Offsets::FNameEntryToString;
    UINT32 Block = blockId;
    UINT16 Offset = 0;
    while (It < End) {
        uintptr_t FNameEntry = It;
        INT16 FNameEntryHeader = Read<INT16>(FNameEntry + Offsets::FNameEntryHeader);
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryToLenBit;
        if (StrLength) {
            bool wide = FNameEntryHeader & 1;

            ///Unicode Dumping Not Supported
            if (StrLength > 0) {
                //String Length Limit
                if (StrLength < 250) {
                    UINT32 key = (Block << 16 | Offset);
                    uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryToString;

                    std::string Names = wide ? WideStr::getString(StrPtr, StrLength) : ReadStringNew(StrPtr, StrLength);

                    if (Offsets::isXorDecrypt) {
                        Dump << (wide ? "Wide \n" : "") << "[" << count << "]: " << DecryptXorCypher(Names) << " | Offset - 0x" << key << " | Length - " << StrLength << "\n";
                    } else {
                        Dump << (wide ? "Wide \n" : "") << "[" << count << "]: " << Names << " | Offset - 0x" << key << " | Length - " << StrLength << "\n";
                    }
                    count++;
                }
            }
            else {
                StrLength = -StrLength;//Negative lengths are for Unicode Characters
            }

            //Next
            Offset += StrLength / Offsets::FNameStride;
            UINT16 bytes = Offsets::FNameEntryToString + StrLength * (wide ? sizeof(wchar_t) : sizeof(char));
            It += (bytes + Offsets::FNameStride - 1u) & ~(Offsets::FNameStride - 1u);
        }
        else {// Null-terminator entry found
            break;
        }
    }
}

void DumpStrings(std::string out) {
    int count = 0;
    std::cout << "\n[*] Dumping Strings from FName" << std::endl;
    std::ofstream Dump(out + "/FNameStrings.txt", std::ofstream::out);
    if (Dump.is_open()) {
        if (Offsets::isUE423) {
            uintptr_t FNamePool = (Memory.ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool;

            UINT32 BlockSize = Offsets::FNameStride * 65536;
            UINT32 CurrentBlock = Read<UINT32>(FNamePool + Offsets::FNamePoolToCurrentBlock);
            UINT32 CurrentByteCursor = Read<UINT32>(FNamePool + Offsets::FNamePoolToCurrentByteCursor);

            for (UINT32 BlockIdx = 0; BlockIdx < CurrentBlock; ++BlockIdx) {
                DumpBlocks(FNamePool, BlockIdx, BlockSize, count, Dump);
            }

            DumpBlocks(FNamePool, CurrentBlock, CurrentByteCursor, count, Dump);
        } else {
            for (UINT32 i = 0; i < 170000; i++) {
                std::string s = GetNameFromFName(i);
                if (!s.empty()) {
                    if (Offsets::isXorDecrypt) {
                        Dump << "[" << count << "]: " << DecryptXorCypher(s) << "\n";
                    } else {
                        Dump << "[" << count << "]: " << s << "\n";
                    }
                    count++;
                }
            }
        }
        Dump.close();
    }
    std::cout << "[*] Dumped FNameStrings...\n" << std::endl;
}

void DumpObjects(std::string out) {
    int Count = 0;
    std::cout << "\n[*] Dumping Objects From GUObjects..." << std::endl;
    std::ofstream Dump(out + "/Objects.txt", std::ofstream::out);
    if (Dump.is_open()) {
        INT32 ObjectsCount = GetObjectCount();
        if (ObjectsCount < 10 || ObjectsCount > 999999) {
            ObjectsCount = 300000;
        }
        for (INT32 i = 0; i < ObjectsCount; i++) {
            uintptr_t ObjectPointer = GetUObjectFromID(i);
            if (!UObject::isValid(ObjectPointer)) continue;
            Dump << "[0x" << std::hex << i <<
                "]: Name: " << UObject::getName(ObjectPointer) <<
                " | Class: " << UObject::getClassName(ObjectPointer) <<
                " | ObjectPtr: 0x" << std::hex << ObjectPointer <<
                " | ClassPtr: 0x" << std::hex << UObject::getClassPrivate(ObjectPointer) << "\n";
            Count++;
        }
        Dump.close();
    }
    std::cout << "[*] Dumped GUObjects...\n" << std::endl;
}

void DumpSDK(std::string out) {
    std::cout << "\n[*] Dumping SDK using GUObjects..." << std::endl;
    std::ofstream Dump(out + "/SDK.txt", std::ofstream::out);

    if (Dump.is_open()) {
        INT32 ObjectsCount = GetObjectCount();
        if (ObjectsCount < 10 || ObjectsCount > 999999) {
            ObjectsCount = 300000;
        }
        for (INT32 i = 0; i < ObjectsCount; i++) {
            uintptr_t ObjectPointer = GetUObjectFromID(i);
            if (!UObject::isValid(ObjectPointer)) continue;
            DumpStructures(Dump, UObject::getClassPrivate(ObjectPointer));
        }
        Dump.close();
    }
    std::cout << "[*] Dumped SDK using GUObjects...\n" << std::endl;
}

void DumpSDKW(std::string out) {
    std::cout << "\n[*] Dumping SDKW using GWorld..." << std::endl;
    std::ofstream Dump(out + "/SDKW.txt", std::ofstream::out);
    if (Dump.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(Memory.ModuleBase + Offsets::GWorld);
        if (!UObject::isValid(GWorld)) {
            std::cout << "[!] GWorld is Invalid..." << std::endl;
            return;
        }

        DumpStructures(Dump, UObject::getClassPrivate(GWorld));

        uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
        uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
        int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);
        for (int i = 0; i < AActorsCount; i++) {
            uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
            if (UObject::isValid(Base)) {
                DumpStructures(Dump, UObject::getClassPrivate(Base));
            }
        }
        Dump.close();
    }
    std::cout << "[*] Dumped SDKW using GWorld...\n" << std::endl;
}

void DumpActors(std::string out) {
    int count = 0;
    std::cout << "\n[*] Dumping Actors from World..." << std::endl;
    std::ofstream Dump(out + "/ActorsDump.txt", std::ofstream::out);
    if (Dump.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(Memory.ModuleBase + Offsets::GWorld);
        if (!UObject::isValid(GWorld)) {
            std::cout << "[!] GWorld is Invalid..." << std::endl;
            return;
        }

        uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::UWorldToPersistentLevel);
        uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::ULevelToAActors);
        int AActorsCount = Read<int>(PersistentLevel + Offsets::ULevelToAActorsCount);

        for (int i = 0; i < AActorsCount; i++) {
            uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
            if (UObject::isValid(Base)) {
                Dump << "[" << count << "]: Name - " << UObject::getName(Base) << ", Address - 0x" << std::hex << Base << "\n";
                count++;
            }
        }
        Dump.close();
    }
    std::cout << "[*] Dumped Actors from World...\n" << std::endl;
}

void BonesDump(std::string out) {
    int count = 0;
    std::cout << "\n[*] Dumping Bones..." << std::endl;
    std::ofstream Dump(out + "/BonesDump.txt", std::ofstream::out);
    if (Dump.is_open()) {
        uintptr_t GWorld = Read<uintptr_t>(Memory.ModuleBase + Offsets::GWorld);
        if (!UObject::isValid(GWorld)) {
            std::cout << "[!] GWorld is Invalid..." << std::endl;
            return;
        }

        uintptr_t OwningGameInstance = Read<uintptr_t>(GWorld + 0x330); // GameInstance* OwningGameInstance; - Class: World.Object
        uintptr_t LocalPlayers = Read<uintptr_t>(OwningGameInstance + 0x38);// LocalPlayer* [] LocalPlayers; - Class: GameInstance.Object
        uintptr_t LocalPlayer = Read<uintptr_t>(LocalPlayers + 0);
        uintptr_t PlayerController = Read<uintptr_t>(LocalPlayer + 0x30); // PlayerController* PlayerController; - Class: Player.Object
        uintptr_t AcknowledgedPawn = Read<uintptr_t>(PlayerController + 0x2c8); // Pawn* AcknowledgedPawn; - Class: PlayerController.Controller.Actor.Object

        uintptr_t Mesh = Read<uintptr_t>(AcknowledgedPawn + 0x2a8); // SkeletalMeshComponent* Mesh; - Class: Character.Pawn.Actor.Object

        uintptr_t SkeletalMesh = Read<uintptr_t>(Mesh + 0x4b0); // SkeletalMesh* SkeletalMesh; - Class: SkinnedMeshComponent.MeshComponent.PrimitiveComponent.SceneComponent.ActorComponent.Object

        uintptr_t Skeleton = Read<uintptr_t>(SkeletalMesh + 0x88); // Skeleton* Skeleton; - Class: SkeletalMesh.StreamableRenderAsset.Object

        uintptr_t Sockets = Read<uintptr_t>(Skeleton + 0x190); // SkeletalMeshSocket*[] Sockets; - Class: Skeleton.Object
        
        int32_t SocketCount = Read<int32_t>(Skeleton + 0x198); // As Sockets is a TArray it has the count 8 byes from the data so (Sockets + 0x8)

        Dump << SocketCount << " bones found in total" << "\n\n";

        for (int x = 0; x < SocketCount; x++) {
            uintptr_t Socket = Read<uintptr_t>(Sockets + (x * Offsets::PointerSize));

            uintptr_t SocketNameIndex = Read<uintptr_t>(Socket + 0x28); // FName SocketName; - Class: SkeletalMeshSocket.Object
            std::string SocketName = GetNameFromFName(SocketNameIndex);

            uintptr_t BoneNameIndex = Read<uintptr_t>(Socket + 0x30);// FName BoneName; - Class: SkeletalMeshSocket.Object
            std::string BoneName = GetNameFromFName(BoneNameIndex);

            if (!BoneName.empty() && BoneName != "None") {
                Dump << std::left << "[ID: " << x << "] " << std::setw(3) << "| Bone: " << std::setw(35) << BoneName << "| Socket: " << SocketName << "\n";
            }
        }
        Dump.close();
    }
    std::cout << "[*] Dumped Bones...\n" << std::endl;
}
