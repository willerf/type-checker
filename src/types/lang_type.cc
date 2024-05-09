
#include "lang_type.h"

#include <iostream>
#include <iterator>

TypeError::TypeError(const LTypeImpl& t1, const LTypeImpl& t2) :
    t1 {t1},
    t2 {t2} {}

PtrLType make_lt(LPrim lprim) {
    auto ltype = std::make_shared<LTypeImpl>(lprim);
    return std::make_shared<LType>(ltype);
}

PtrLType make_lt(LTypeClass tc) {
    LGeneric tmp = {tc};
    auto ltype = std::make_shared<LTypeImpl>(tmp);
    return std::make_shared<LType>(ltype);
}

PtrLType make_lt(LGeneric tcs) {
    auto ltype = std::make_shared<LTypeImpl>(tcs);
    return std::make_shared<LType>(ltype);
}

PtrLType make_lt(LCustom lcustom) {
    std::cerr << "todo" << std::endl;
    exit(1);
}

PtrLType make_lt(LTypeImpl ltypeimpl) {
    auto ltype = std::make_shared<LTypeImpl>(ltypeimpl);
    return std::make_shared<LType>(ltype);
}

std::string to_string(LPrim lprim) {
    switch (lprim) {
        case LPrim::Invalid:
            return "invalid";
        case LPrim::Unit:
            return "unit";
        case LPrim::Int:
            return "int";
        case LPrim::Bool:
            return "bool";
        case LPrim::Char:
            return "char";
        case LPrim::Str:
            return "str";
    }
}

std::string to_string(LTypeClass tc) {
    switch (tc) {
        case LTypeClass::Plus:
            return "Plus";
        case LTypeClass::Minus:
            return "Minus";
        case LTypeClass::Star:
            return "Star";
        case LTypeClass::Slash:
            return "Slash";
        case LTypeClass::Percent:
            return "Percent";
        case LTypeClass::Eq:
            return "Eq";
        case LTypeClass::Ord:
            return "Ord";
    }
}

std::string to_string(LGeneric tcs) {
    std::string result = "[";
    for (auto tc : tcs) {
        result += to_string(tc) + ", ";
    }
    if (!tcs.empty()) {
        result.pop_back();
        result.pop_back();
    }
    result += "]";
    return result;
}

std::string to_string(LCustom lcustom) {
    std::string result = lcustom.name;
    result += "(";
    for (auto child : lcustom.children) {
        result += to_string(child) + ", ";
    }
    if (!lcustom.children.empty()) {
        result.pop_back();
        result.pop_back();
    }
    result += ")";
    return result;
}

std::string to_string(LTypeImpl ltypeimpl) {
    if (std::holds_alternative<LPrim>(ltypeimpl)) {
        return to_string(std::get<LPrim>(ltypeimpl));
    }
    if (std::holds_alternative<LGeneric>(ltypeimpl)) {
        return to_string(std::get<LGeneric>(ltypeimpl));
    }
    if (std::holds_alternative<LCustom>(ltypeimpl)) {
        return to_string(std::get<LCustom>(ltypeimpl));
    }
    return "Unreachable";
}

std::string to_string(LType ltype) {
    return to_string(*ltype);
}

std::string to_string(PtrLType ptrltype) {
    return to_string(*ptrltype);
}
