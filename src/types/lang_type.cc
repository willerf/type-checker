
#include "lang_type.h"

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

std::string to_string(LType ltype) {
    if (std::holds_alternative<LPrim>(ltype)) {
        return to_string(std::get<LPrim>(ltype));
    }
    if (std::holds_alternative<std::vector<LTypeClass>>(ltype)) {
        std::string result = "";
        for (auto tc : std::get<std::vector<LTypeClass>>(ltype)) {
            result += to_string(tc) + " "; 
        }
        result.pop_back();
        return result;
    }
    if (std::holds_alternative<LCustom>(ltype)) {
        return "CustomType";
    }
    return "Unreachable";
}
