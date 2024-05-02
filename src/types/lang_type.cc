
#include "lang_type.h"

std::string to_string(LPrim lprim) {
    switch (lprim) {
        case LPrim::Int:
            return "Int";
        case LPrim::Bool:
            return "Bool";
        case LPrim::Generic:
            return "Generic";
    }
}

std::string to_string(LType ltype) {
    if (std::holds_alternative<LPrim>(ltype)) {
        return to_string(std::get<LPrim>(ltype));
    }
    if (std::holds_alternative<LCustom>(ltype)) {
        return "CustomType";
    }
    return "Unreachable";
}
