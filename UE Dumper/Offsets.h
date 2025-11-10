#pragma once

#include "Includes.h"

namespace Offsets {
    uintptr_t PointerSize = sizeof(void*);

    bool isUE423 = false, isDereferencing = false, isXorDecrypt = false;

    // EngineOffsets
    uintptr_t GWorld = 0x0;
    uintptr_t GNames = 0x0;
    uintptr_t GUObjectArray = 0x0; // whatever found in IDA do a (minus) - FUObjectArrayToTUObjectArray to get the value or set FUObjectArrayToTUObjectArray to 0

    // TNameEntry
    uintptr_t TNameEntryToNameString = 0x10;

    // FNamePool
    uintptr_t GNamesToFNamePool = 0x0;
    uintptr_t FNamePoolToBlocks = 0x10;
    uintptr_t FNameStride = 0x2; // sometimes 0x4 in UE5.0+
    uintptr_t FNamePoolToCurrentBlock = 0x8;
    uintptr_t FNamePoolToCurrentByteCursor = 0xC;

    // FNameEntry
    uintptr_t FNameEntryHeader = 0x0;
    uintptr_t FNameEntryToLenBit = 0x6;
    uintptr_t FNameEntryToString = 0x2;

    // FUObjectArray
    uintptr_t FUObjectArrayToTUObjectArray = 0x10; // 0x10 in UE4.23+ but its already calculated at runtime in most games so set this to 0 to minus from GUObjectArray
    uintptr_t FUObjectItemPadd = 0x0;
    uintptr_t FUObjectItemSize = 0x18;

    // TUObjectArray
    uintptr_t TUObjectArrayToNumElements = 0x14;

    // UObject
    uintptr_t UObjectToInternalIndex = 0xC;
    uintptr_t UObjectToClassPrivate = 0x10;
    uintptr_t UObjectToFNameIndex = UObjectToClassPrivate + PointerSize;
    uintptr_t UObjectToOuterPrivate = UObjectToFNameIndex + PointerSize;

    // FField
    uintptr_t FFieldToClass = 0x8;
    uintptr_t FFieldToNext = FFieldToClass + 0x10;
    uintptr_t FFieldToName = FFieldToNext + PointerSize;

    // UField
    uintptr_t UFieldToNext = 0x28;

    // UStruct
    uintptr_t UStructToSuperStruct = 0x40;
    uintptr_t UStructToChildren = UStructToSuperStruct + PointerSize;
    uintptr_t UStructToChildProperties = 0x50;

    // UFunction
    uintptr_t UFunctionToFunctionFlags = 0xB0;
    uintptr_t UFunctionToFunc = UFunctionToFunctionFlags + UFieldToNext;

    // UProperty
    uintptr_t UPropertyToElementSize = 0x34;
    uintptr_t UPropertyToPropertyFlags = UPropertyToElementSize + 0x4;
    uintptr_t UPropertyToOffsetInternal = 0x44;

    // UBoolProperty
    uintptr_t UBoolPropertyToFieldSize = 0x70;
    uintptr_t UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
    uintptr_t UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
    uintptr_t UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

    // UObjectProperty
    uintptr_t UObjectPropertyToPropertyClass = UBoolPropertyToFieldSize;

    // UMapProperty
    uintptr_t UMapPropertyToKeyProp = UBoolPropertyToFieldSize;
    uintptr_t UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

    // USetProperty
    uintptr_t USetPropertyToElementProp = UBoolPropertyToFieldSize;

    // UStructProperty
    uintptr_t UStructPropertyToStruct = UBoolPropertyToFieldSize;

    // UArrayProperty
    uintptr_t UArrayPropertyToInnerProperty = UBoolPropertyToFieldSize + PointerSize;

    // UClassProperty
    uintptr_t UClassPropertyToMetaClass = UBoolPropertyToFieldSize + PointerSize;

    // UInterfaceProperty
    uintptr_t UInterfacePropertyToUInterface = UBoolPropertyToFieldSize + PointerSize;

    // UWorld
    uintptr_t UWorldToPersistentLevel = 0x30;

    // ULevel
    uintptr_t ULevelToAActors = 0x98;
    uintptr_t ULevelToAActorsCount = ULevelToAActors + PointerSize;

    /*
    Most of the time the Offsets for 
    UBoolPropertyToFieldSize, UObjectPropertyToPropertyClass, UMapPropertyToKeyProp, USetPropertyToElementProp & UStructPropertyToStruct are all same
    sometimes even UArrayPropertyToInnerProperty

    and 
    
    UArrayPropertyToInnerProperty, UClassPropertyToMetaClass, UInterfacePropertyToUInterface are all same

    Also most of the time (Almost 100% for me) UArrayPropertyToInnerProperty, UClassPropertyToMetaClass, UInterfacePropertyToUInterface are +0x8 from UBoolPropertyToFieldSize
    
    */

    void ReadyOrNot() {
        Memory.GameName = "ReadyOrNot";
        Memory.TargetProcess = "ReadyOrNotSteam-Win64-Shipping.exe";

        isUE423 = true;

        GWorld = 0x90F4AD8;
        GNames = 0x8EE1B80;
        GUObjectArray = 0x8F87F20;
    }

    void Bloodlines2() {
        Memory.GameName = "Bloodlines2";
        Memory.TargetProcess = "Bloodlines2-Win64-Shipping.exe";

        isUE423 = true;

        GWorld = 0x805F138;
        GNames = 0x7E4BDC0;
        GUObjectArray = 0x7EF21A0;
    }

    void DarkHours() {
        Memory.GameName = "DarkHours";
        Memory.TargetProcess = "darkhours-Win64-Shipping.exe";

        isUE423 = true;

        GWorld = 0x8AD7648;
        GNames = 0x889C5C0;
        GUObjectArray = 0x8953990;
    }

    void BlackStigma() {
        Memory.GameName = "BlackStigma";
        Memory.TargetProcess = "StigmaClient-Win64-Shipping.exe";

        isUE423 = true;

        GWorld = 0x7A98F98;
        GNames = 0x7884A40;
        GUObjectArray = 0x792AE20;
    }

    void DarkestDays() {
        Memory.GameName = "DarkestDays";
        Memory.TargetProcess = "ProjectNow-Win64-Shipping.exe";

        isUE423 = true;

        GWorld = 0x69EDF60;
        GNames = 0x686AF40;
        GUObjectArray = 0x68A7D90;

        FFieldToNext = FFieldToClass + 0x18;
        FFieldToName = FFieldToNext + PointerSize;

        UPropertyToElementSize = 0x3C;
        UPropertyToPropertyFlags = UPropertyToElementSize + 0x4;
        UPropertyToOffsetInternal = 0x4C;

        UBoolPropertyToFieldSize = 0x78;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        UObjectPropertyToPropertyClass = UBoolPropertyToFieldSize;

        UMapPropertyToKeyProp = UBoolPropertyToFieldSize;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        USetPropertyToElementProp = UBoolPropertyToFieldSize;

        UStructPropertyToStruct = UBoolPropertyToFieldSize;

        UArrayPropertyToInnerProperty = UBoolPropertyToFieldSize;

        UClassPropertyToMetaClass = UBoolPropertyToFieldSize;

        UInterfacePropertyToUInterface = UBoolPropertyToFieldSize;
    }

    void Mir4S() {
        Memory.GameName = "Mir4S";
        Memory.TargetProcess = "Mir4S.exe";

        isUE423 = true;

        GWorld = 0x8289208;
        GNames = 0x81017C0;
        GUObjectArray = 0x813DE50;

        FFieldToNext = FFieldToClass + 0x18;
        FFieldToName = FFieldToNext + PointerSize;

        UPropertyToElementSize = 0x3C; 
        UPropertyToPropertyFlags = UPropertyToElementSize + 0x4;
        UPropertyToOffsetInternal = 0x4C;

        UBoolPropertyToFieldSize = 0x78;
        UBoolPropertyToByteOffset = UBoolPropertyToFieldSize + 0x1;
        UBoolPropertyToByteMask = UBoolPropertyToByteOffset + 0x1;
        UBoolPropertyToFieldMask = UBoolPropertyToByteMask + 0x1;

        UObjectPropertyToPropertyClass = UBoolPropertyToFieldSize;

        UMapPropertyToKeyProp = UBoolPropertyToFieldSize;
        UMapPropertyToValueProp = UMapPropertyToKeyProp + PointerSize;

        USetPropertyToElementProp = UBoolPropertyToFieldSize;

        UStructPropertyToStruct = UBoolPropertyToFieldSize;

        UArrayPropertyToInnerProperty = UBoolPropertyToFieldSize;

        UClassPropertyToMetaClass = UBoolPropertyToFieldSize;

        UInterfacePropertyToUInterface = UBoolPropertyToFieldSize;
    }
};