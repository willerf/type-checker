
#include "lang_type.h"

#include <iostream>

PtrLType make_lt(LPrim lprim) {
    auto ltype = std::make_shared<LTypeImpl>(lprim);
    return std::make_shared<LType>(ltype);
}

PtrLType make_lt(LTypeClass tc) {
    std::set<LTypeClass> tmp = {tc};
    auto ltype = std::make_shared<LTypeImpl>(tmp);
    return std::make_shared<LType>(ltype);
}

PtrLType make_lt(LCustom lcustom) {
    std::cerr << "Implement this" << std::endl;
    exit(1);
}

PtrLType make_lt(LTypeImpl ltypeimpl) {
    auto ltype = std::make_shared<LTypeImpl>(ltypeimpl);
    return std::make_shared<LType>(ltype);
}

std::string to_string(LPrim lprim) {
    switch (lprim) {
        case LPrim::Invalid:
            return "Invalid";
        case LPrim::Int:
            return "Int";
        case LPrim::Bool:
            return "Bool";
        case LPrim::Generic:
            return "Generic";
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
    if (std::holds_alternative<std::set<LTypeClass>>(ltypeimpl)) {
        auto& tcs = std::get<std::set<LTypeClass>>(ltypeimpl);
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

