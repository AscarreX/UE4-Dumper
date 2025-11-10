#pragma once

#include "Structures.h"

bool isStartWith(std::string str, const char* check) {
    return (str.rfind(check, 0) == 0);
}

bool isEqual(std::string s1, const char* check) {
    std::string s2(check);
    return (s1 == s2);
}

bool isScanned(UINT32 id) {
    for (int i = 0; i < structIDMap.size(); i++) {
        if (structIDMap[i] == id) { return true; }
    }
    return false;
}

bool isContain(std::string str, std::string check) {
    size_t found = str.find(check);
    return (found != std::string::npos);
}

std::string ResolveProperty(std::list<uintptr_t>& recurrce, uintptr_t prop) {
    if (!prop) return "NULL";

    std::string ClassName = Offsets::isUE423 ? FField::getClassName(prop) : UObject::getClassName(prop);
    std::string PropertyName = Offsets::isUE423 ? FField::getName(prop) + "(" + ClassName + ")" : UObject::getName(prop) + "(" + ClassName + ")";

    static const std::unordered_map<std::string,
        std::function<std::string(uintptr_t, std::list<uintptr_t>&)>> handlers = {

        {"ObjectProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t cls = UObjectProperty::getPropertyClass(p);
            r.push_back(cls);
            return UObject::getName(cls) + "*";
        }},
        {"WeakObjectProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t cls = UObjectProperty::getPropertyClass(p);
            r.push_back(cls);
            return UObject::getName(cls) + "*";
        }},
        {"LazyObjectProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t cls = UObjectProperty::getPropertyClass(p);
            r.push_back(cls);
            return UObject::getName(cls) + "*";
        }},
        {"AssetObjectProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t cls = UObjectProperty::getPropertyClass(p);
            r.push_back(cls);
            return UObject::getName(cls) + "*";
        }},
        {"SoftObjectProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t cls = UObjectProperty::getPropertyClass(p);
            r.push_back(cls);
            return UObject::getName(cls) + "*";
        }},

        {"ClassProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t meta = UClassProperty::getMetaClass(p);
            r.push_back(meta);
            return "class " + UObject::getName(meta);
        }},
        {"AssetClassProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t meta = UClassProperty::getMetaClass(p);
            r.push_back(meta);
            return "class " + UObject::getName(meta);
        }},
        {"SoftClassProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t meta = UClassProperty::getMetaClass(p);
            r.push_back(meta);
            return "class " + UObject::getName(meta);
        }},

        {"InterfaceProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t iface = UInterfaceProperty::getInterfaceClass(p);
            r.push_back(iface);
            return "interface class " + UObject::getName(iface);
        }},

        {"StructProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            uintptr_t s = UStructProperty::getStruct(p);
            r.push_back(s);
            return UObject::getName(s);
        }},

        {"ArrayProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            return ResolveProperty(r, UArrayProperty::getInner(p)) + "[]";
        }},

        {"SetProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            return "<" + ResolveProperty(r, USetProperty::getElementProp(p)) + ">";
        }},

        {"MapProperty", [](uintptr_t p, std::list<uintptr_t>& r) {
            return "<" + ResolveProperty(r, UMapProperty::getKeyProp(p)) + "," +
                ResolveProperty(r, UMapProperty::getValueProp(p)) + ">";
        }},

        {"BoolProperty", [](auto, auto&) { return "bool"; }},
        {"ByteProperty", [](auto, auto&) { return "byte"; }},
        {"IntProperty", [](auto, auto&) { return "int"; }},
        {"Int8Property", [](auto, auto&) { return "int8"; }},
        {"Int16Property", [](auto, auto&) { return "int16"; }},
        {"Int64Property", [](auto, auto&) { return "int64"; }},
        {"UInt16Property", [](auto, auto&) { return "uint16"; }},
        {"UINT32Property", [](auto, auto&) { return "UINT32"; }},
        {"UINT64Property", [](auto, auto&) { return "UINT64"; }},
        {"DoubleProperty", [](auto, auto&) { return "double"; }},
        {"FloatProperty", [](auto, auto&) { return "float"; }},
        {"EnumProperty", [](auto, auto&) { return "enum"; }},
        {"StrProperty", [](auto, auto&) { return "FString"; }},
        {"TextProperty", [](auto, auto&) { return "FText"; }},
        {"NameProperty", [](auto, auto&) { return "FName"; }},
        {"DelegateProperty", [](auto, auto&) { return "delegate"; }},
        {"MulticastDelegateProperty", [](auto, auto&) { return "delegate"; }}
    };

    auto it = handlers.find(ClassName);
    if (it != handlers.end())
        return it->second(prop, recurrce);

    return PropertyName;
}

std::list<uintptr_t> WriteChildStructuresCombo(std::ofstream& sdk, uintptr_t childprop) {
    std::list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        std::string oname = Offsets::isUE423 ? FField::getName(prop) : UObject::getName(prop);
        std::string cname = Offsets::isUE423 ? FField::getClassName(prop) : UObject::getClassName(prop);

    }
}

std::list<uintptr_t> WriteChildStructures(std::ofstream& sdk, uintptr_t childprop) {
    std::list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        std::string oname = UObject::getName(prop);
        std::string cname = UObject::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty") ||
            isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty") ||
            isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);

            sdk << "\t" << UObject::getName(propertyClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(propertyClass);
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);

            sdk << "\tclass " << UObject::getName(metaClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);

            sdk << "\tinterface class " << UObject::getName(interfaceClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);

            sdk << "\t" << UObject::getName(Struct) << " " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            sdk << "\t" << ResolveProperty(recurrce, UArrayProperty::getInner(prop)) << "[] " << oname
                << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "SetProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, USetProperty::getElementProp(prop)) << "> "
                << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "MapProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, UMapProperty::getKeyProp(prop)) << ","
                << ResolveProperty(recurrce, UMapProperty::getValueProp(prop)) << "> " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "BoolProperty")) {
            sdk << "\tbool " << oname << ";"
                << std::setbase(10) << "//(ByteOffset: " << (int)UBoolProperty::getByteOffset(prop)
                << ", ByteMask: " << (int)UBoolProperty::getByteMask(prop)
                << ", FieldMask: " << (int)UBoolProperty::getFieldMask(prop) << ")"
                << "[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "ByteProperty")) {
            sdk << "\tbyte " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "IntProperty")) {
            sdk << "\tint " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int8Property")) {
            sdk << "\tint8 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int16Property")) {
            sdk << "\tint16 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int64Property")) {
            sdk << "\tint64 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UInt16Property")) {
            sdk << "\tuint16 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UINT32Property")) {
            sdk << "\tUINT32 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UINT64Property")) {
            sdk << "\tUINT64 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "DoubleProperty")) {
            sdk << "\tdouble " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "FloatProperty")) {
            sdk << "\tfloat " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "EnumProperty")) {
            sdk << "\tenum " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "StrProperty")) {
            sdk << "\tFString " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "TextProperty")) {
            sdk << "\tFText " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "NameProperty")) {
            sdk << "\tFName " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty") ||
            isEqual(cname, "MulticastInlineDelegateProperty") ||
            isEqual(cname, "MulticastSparseDelegateProperty")) {
            sdk << "\tdelegate " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "XigPtrProperty")) {
            sdk << "\tXigPtrProperty " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isStartWith(cname, "Function") || isEqual(cname, "DelegateFunction")) {
            std::string returnVal = "void";
            std::string params = "";

            uintptr_t funcParam = UStruct::getChildren(prop);
            while (funcParam) {
                UINT64 PropertyFlags = UProperty::getPropertyFlags(funcParam);

                if ((PropertyFlags & 0x0000000000000400) == 0x0000000000000400) {
                    returnVal = ResolveProperty(recurrce, funcParam);
                }
                else {
                    if ((PropertyFlags & 0x0000000000000100) == 0x0000000000000100) {
                        params += "out ";
                    }
                    if((PropertyFlags & 0x0000000008000000) == 0x0000000008000000){
                        params += "ref ";
                    }
                    if ((PropertyFlags & 0x0000000000000002) == 0x0000000000000002) {
                        params += "const ";
                    }
                    params += ResolveProperty(recurrce, funcParam);
                    params += " ";
                    params += UObject::getName(funcParam);
                    params += ", ";
                }

                funcParam = UField::getNext(funcParam);
            }

            if (!params.empty()) {
                params.pop_back();
                params.pop_back();
            }

            sdk << "\t";

            INT32 FunctionFlags = UFunction::getFunctionFlags(prop);

            if ((FunctionFlags & 0x00002000) == 0x00002000) {
                sdk << "static" << " ";
            }
            if((FunctionFlags & 0x00000001) == 0x00000001){
                sdk << "final" << " ";
            }
            if((FunctionFlags & 0x00020000) == 0x00020000){
                sdk << "public" << " ";
            }
            if((FunctionFlags & 0x00040000) == 0x00040000){
                sdk << "private" << " ";
            }
            if((FunctionFlags & 0x00080000) == 0x00080000){
                sdk << "protected" << " ";
            }

            sdk << returnVal << " " << oname << "(" << params << ");"
                << "// 0x" << std::setbase(16) << (UFunction::getFunc(prop) - Memory.ModuleBase) << "\n";
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }

        child = UField::getNext(child);
    }
    return recurrce;
}

std::list<uintptr_t> WriteChildStructuresNew(std::ofstream& sdk, uintptr_t childprop) {
    std::list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        std::string oname = FField::getName(prop);
        std::string cname = FField::getClassName(prop);

        if (isEqual(cname, "ObjectProperty") || isEqual(cname, "WeakObjectProperty") ||
            isEqual(cname, "LazyObjectProperty") || isEqual(cname, "AssetObjectProperty") ||
            isEqual(cname, "SoftObjectProperty")) {
            uintptr_t propertyClass = UObjectProperty::getPropertyClass(prop);

            sdk << "\t" << UObject::getName(propertyClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(propertyClass);
        }
        else if (isEqual(cname, "ClassProperty") || isEqual(cname, "AssetClassProperty") ||
            isEqual(cname, "SoftClassProperty")) {
            uintptr_t metaClass = UClassProperty::getMetaClass(prop);

            sdk << "\tclass " << UObject::getName(metaClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(metaClass);
        }
        else if (isEqual(cname, "InterfaceProperty")) {
            uintptr_t interfaceClass = UInterfaceProperty::getInterfaceClass(prop);

            sdk << "\tinterface class " << UObject::getName(interfaceClass) << "* " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "StructProperty")) {
            uintptr_t Struct = UStructProperty::getStruct(prop);

            sdk << "\t" << UObject::getName(Struct) << " " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";

            recurrce.push_back(Struct);
        }
        else if (isEqual(cname, "ArrayProperty")) {
            sdk << "\t" << ResolveProperty(recurrce, UArrayProperty::getInner(prop)) << "[] "
                << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "SetProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, USetProperty::getElementProp(prop)) << "> "
                << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "MapProperty")) {
            sdk << "\t<" << ResolveProperty(recurrce, UMapProperty::getKeyProp(prop)) << ","
                << ResolveProperty(recurrce, UMapProperty::getValueProp(prop)) << "> " << oname
                << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "BoolProperty")) {
            sdk << "\tbool " << oname << ";"
                << std::setbase(10) << "//(ByteOffset: " << (int)UBoolProperty::getByteOffset(prop)
                << ", ByteMask: " << (int)UBoolProperty::getByteMask(prop)
                << ", FieldMask: " << (int)UBoolProperty::getFieldMask(prop) << ")"
                << "[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "ByteProperty")) {
            sdk << "\tbyte " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "IntProperty")) {
            sdk << "\tint " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int8Property")) {
            sdk << "\tint8 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int16Property")) {
            sdk << "\tint16 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "Int64Property")) {
            sdk << "\tint64 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UInt16Property")) {
            sdk << "\tuint16 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UINT32Property")) {
            sdk << "\tUINT32 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "UINT64Property")) {
            sdk << "\tUINT64 " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "DoubleProperty")) {
            sdk << "\tdouble " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "FloatProperty")) {
            sdk << "\tfloat " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "EnumProperty")) {
            sdk << "\tenum " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "StrProperty")) {
            sdk << "\tFString " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "TextProperty")) {
            sdk << "\tFText " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "NameProperty")) {
            sdk << "\tFName " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "DelegateProperty") ||
            isEqual(cname, "MulticastDelegateProperty") ||
            isEqual(cname, "MulticastInlineDelegateProperty") ||
            isEqual(cname, "MulticastSparseDelegateProperty")) {
            sdk << "\tdelegate " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else if (isEqual(cname, "XigPtrProperty")) {
            sdk << "\tXigPtrProperty " << oname << ";"
                << "//[Offset: 0x" << std::setbase(16) << UProperty::getOffset(prop) << ", "
                << "Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }

        child = FField::getNext(child);
    }
    return recurrce;
}

std::list<uintptr_t> WriteChildStructuresNew_Func(std::ofstream& sdk, uintptr_t childprop) {
    std::list<uintptr_t> recurrce;
    uintptr_t child = childprop;
    while (child) {
        uintptr_t prop = child;
        std::string oname = UObject::getName(prop);
        std::string cname = UObject::getClassName(prop);

        if (isStartWith(cname, "Function") || isEqual(cname, "DelegateFunction")) {
            std::string returnVal = "void";
            std::string params = "";

            uintptr_t funcParam = UStruct::getChildProperties(prop);
            while (funcParam) {
                UINT64 PropertyFlags = UProperty::getPropertyFlags(funcParam);

                if ((PropertyFlags & 0x0000000000000400) == 0x0000000000000400) {
                    returnVal = ResolveProperty(recurrce, funcParam);
                }
                else {
                    if ((PropertyFlags & 0x0000000000000100) == 0x0000000000000100) {
                        params += "out ";
                    }
                    /*if ((PropertyFlags & 0x0000000008000000) == 0x0000000008000000) {
                        params += "ref ";
                    }*/
                    if ((PropertyFlags & 0x0000000000000002) == 0x0000000000000002) {
                        params += "const ";
                    }
                    params += ResolveProperty(recurrce, funcParam);
                    params += " ";
                    params += FField::getName(funcParam);
                    params += ", ";
                }

                funcParam = FField::getNext(funcParam);
            }

            if (!params.empty()) {
                params.pop_back();
                params.pop_back();
            }

            sdk << "\t";

            INT32 FunctionFlags = UFunction::getFunctionFlags(prop);

            if ((FunctionFlags & 0x00002000) == 0x00002000) {
                sdk << "static" << " ";
            }
            /*if ((FunctionFlags & 0x00000001) == 0x00000001) {
                sdk << "final" << " ";
            }
            if((FunctionFlags & 0x00020000) == 0x00020000){
                sdk << "public" << " ";
            }
            if((FunctionFlags & 0x00040000) == 0x00040000){
                sdk << "private" << " ";
            }
            if((FunctionFlags & 0x00080000) == 0x00080000){
                sdk << "protected" << " ";
            }*/

            sdk << returnVal << " " << oname << "(" << params << ");"
                << "// 0x" << std::setbase(16) << (UFunction::getFunc(prop) - Memory.ModuleBase) << "\n";
        }
        else {
            sdk << "\t" << cname << " " << oname << ";"
                << "//[Size: 0x" << std::setbase(16) << UProperty::getElementSize(prop) << "]" << "\n";
        }

        child = UField::getNext(child);
    }
    return recurrce;
}

void DumpStructures(std::ofstream& sdk, uintptr_t clazz) {
    std::list<uintptr_t> recurrce;

    uintptr_t currStruct = clazz;
    while (UObject::isValid(currStruct)) {
        std::string name = UObject::getName(currStruct);
        if (isEqual(name, "None") || isContain(name, "/Game/") || isContain(name, "_png") || name.empty()) {
            break;
        }

        UINT32 NameID = UObject::getNameIndex(currStruct);
        if (!isScanned(NameID)) {
            //Dumping
            structIDMap.push_back(NameID);
            sdk << "Class: " << UStruct::getStructClassPath(currStruct) << "\n";

            if (Offsets::isUE423) {
                recurrce.merge(WriteChildStructuresNew(sdk, UStruct::getChildProperties(currStruct)));
                recurrce.merge(WriteChildStructuresNew_Func(sdk, UStruct::getChildren(currStruct)));
            }
            else {
                recurrce.merge(WriteChildStructures(sdk, UStruct::getChildren(currStruct)));
            }
            sdk << "\n--------------------------------" << "\n";
        }

        currStruct = UStruct::getSuperClass(currStruct);
    }

    for (auto it = recurrce.begin(); it != recurrce.end(); ++it)
        DumpStructures(sdk, *it);
}