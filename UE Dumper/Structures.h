#pragma once

#include "GUObjects.h"

struct UObject {
    static INT32 getInternalIndex(uintptr_t object) {
        return Read<INT32>(object + Offsets::UObjectToInternalIndex);
    }

    static uintptr_t getClassPrivate(uintptr_t object) {
        return Read<uintptr_t>(object + Offsets::UObjectToClassPrivate);
    }

    static UINT32 getNameIndex(uintptr_t object) {
        return Read<UINT32>(object + Offsets::UObjectToFNameIndex);
    }

    static uintptr_t getOuterPrivate(uintptr_t object) {
        return Read<uintptr_t>(object + Offsets::UObjectToOuterPrivate);
    }

    static std::string getName(uintptr_t object) {
        return GetNameFromFName(getNameIndex(object));
    }

    static std::string getClassName(uintptr_t object) {
        return getName(getClassPrivate(object));
    }

    static bool isValid(uintptr_t object) {
        return (object > 0 && getNameIndex(object) > 0 && getClassPrivate(object) > 0);
    }

    static uintptr_t getPackageObject(uintptr_t object) {
        uintptr_t package = 0;
        for (uintptr_t outer = getOuterPrivate(object); outer; outer = getOuterPrivate(outer)) {
            package = outer;
        }
        return package;
    }

    static std::string getFullName(uintptr_t object) {
        if (!isValid(object))
            return "Invalid";
        std::string temp;
        for (auto outer = getOuterPrivate(object); outer; outer = getOuterPrivate(outer)) {
            temp = getName(outer) + "." + temp;
        }
        uintptr_t objectClass = getClassPrivate(object);
        std::string name = getName(objectClass) + " " + temp + getName(object);
        return name;
    }

    static uintptr_t FindObject(const std::string& name) {
        INT32 count = GetObjectCount();

        for (INT32 i = 0; i < count; i++) {
            uintptr_t obj = GetUObjectFromID(i);

            if (UObject::isValid(obj)) {
                std::string fullName = getFullName(obj);
                if (fullName == name) {
                    return obj;
                }
            }
        }

        return 0;
    }

    static bool IsA(uintptr_t object, uintptr_t cmpClass) {
        uintptr_t klass = getClassPrivate(object);
        while (klass) {
            if (klass == cmpClass)
                return true;
            klass = Read<uintptr_t>(klass + Offsets::UStructToSuperStruct);
        }
        return false;
    }
};

struct UField {
    static uintptr_t getNext(uintptr_t field) {
        return Read<uintptr_t>(field + Offsets::UFieldToNext);
    }
};

struct FField {
    static std::string getName(uintptr_t fField) {
        return GetNameFromFName(Read<UINT32>(fField + Offsets::FFieldToName));
    }

    static std::string getClassName(uintptr_t fField) {
        return GetNameFromFName(Read<UINT32>(Read<uintptr_t>(fField + Offsets::FFieldToClass)));
    }

    static uintptr_t getNext(uintptr_t fField) {
        return Read<uintptr_t>(fField + Offsets::FFieldToNext);
    }
};

struct UStruct {
    static uintptr_t getSuperClass(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructToSuperStruct);
    }

    static uintptr_t getChildren(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructToChildren);
    }

    static uintptr_t getChildProperties(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructToChildProperties);
    }

    static std::string getClassName(uintptr_t clazz) {
        return UObject::getName(clazz);
    }

    static std::string getClassPath(uintptr_t object) {
        uintptr_t clazz = UObject::getClassPrivate(object);
        std::string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }

    static std::string getStructClassPath(uintptr_t clazz) {
        std::string classname = UObject::getName(clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }
};

struct UFunction {
    static INT32 getFunctionFlags(uintptr_t func) {
        return Read<INT32>(func + Offsets::UFunctionToFunctionFlags);
    }

    static uintptr_t getFunc(uintptr_t func) {
        return Read<uintptr_t>(func + Offsets::UFunctionToFunc);
    }
};

struct UProperty {
    static INT32 getElementSize(uintptr_t prop) {
        return Read<INT32>(prop + Offsets::UPropertyToElementSize);
    }

    static UINT64 getPropertyFlags(uintptr_t prop) {
        return Read<UINT64>(prop + Offsets::UPropertyToPropertyFlags);
    }

    static INT32 getOffset(uintptr_t prop) {
        return Read<INT32>(prop + Offsets::UPropertyToOffsetInternal);
    }
};

struct UBoolProperty {
    static UINT8 getFieldSize(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToFieldSize);
    }

    static UINT8 getByteOffset(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToByteOffset);
    }

    static UINT8 getByteMask(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToByteMask);
    }

    static UINT8 getFieldMask(uintptr_t prop) {
        return Read<UINT8>(prop + Offsets::UBoolPropertyToFieldMask);
    }
};

struct UObjectProperty {
    static uintptr_t getPropertyClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UObjectPropertyToPropertyClass);
    }
};

struct UClassProperty {
    static uintptr_t getMetaClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UClassPropertyToMetaClass);
    }
};

struct UInterfaceProperty {
    static uintptr_t getInterfaceClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UInterfacePropertyToUInterface);
    }
};

struct UArrayProperty {
    static uintptr_t getInner(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UArrayPropertyToInnerProperty);
    }
};

struct UMapProperty {
    static uintptr_t getKeyProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToKeyProp);
    }

    static uintptr_t getValueProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UMapPropertyToValueProp);
    }
};

struct USetProperty {
    static uintptr_t getElementProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::USetPropertyToElementProp);
    }
};

struct UStructProperty {
    static uintptr_t getStruct(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UStructPropertyToStruct);
    }
};