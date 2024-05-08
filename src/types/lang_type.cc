
#include "lang_type.h"

PtrLType make_lt(LPrim lprim) {
    return std::make_shared<LType>(lprim);
}

PtrLType make_lt(LTypeClass tc) {
    std::vector<LTypeClass> tmp = {tc};
    return std::make_shared<LType>(tmp);
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

std::string to_string(LType ltype) {
    if (std::holds_alternative<LPrim>(ltype)) {
        return to_string(std::get<LPrim>(ltype));
    }
    if (std::holds_alternative<std::vector<LTypeClass>>(ltype)) {
        auto& tcs = std::get<std::vector<LTypeClass>>(ltype);
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
    if (std::holds_alternative<LCustom>(ltype)) {
        return to_string(std::get<LCustom>(ltype));
    }
    return "Unreachable";
}

std::string to_string(PtrLType ptrltype) {
    return to_string(*ptrltype);
}

