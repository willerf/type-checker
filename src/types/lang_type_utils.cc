
#include "lang_type_utils.h"

#include "lang_type.h"

bool compatible(LPrim lprim, LGeneric lgeneric) {
    if (lgeneric.empty()) {
        return true;
    }

    LGeneric compat_set;
    std::vector<LTypeClass> result;
    switch (lprim) {
        case LPrim::Invalid:
            return false;
        case LPrim::Unit:
            return false;
        case LPrim::Int:
            compat_set = {
                LTypeClass::Eq,
                LTypeClass::Ord,
                LTypeClass::Plus,
                LTypeClass::Minus,
                LTypeClass::Star,
                LTypeClass::Slash,
                LTypeClass::Percent};
            break;
        case LPrim::Bool:
            compat_set = {LTypeClass::Eq};
            break;
        case LPrim::Char:
            compat_set = {LTypeClass::Eq, LTypeClass::Ord};
            break;
        case LPrim::Str:
            compat_set = {LTypeClass::Eq, LTypeClass::Ord};
            break;
    }
    std::set_difference(
        lgeneric.begin(),
        lgeneric.end(),
        compat_set.begin(),
        compat_set.end(),
        std::back_inserter(result)
    );
    return result.empty();
}

bool compatible(LArray larray, LGeneric lgeneric) {
    if (lgeneric.empty()) {
        return true;
    }

    LGeneric compat_set = {LTypeClass::Eq};
    std::vector<LTypeClass> result;
    std::set_difference(
        lgeneric.begin(),
        lgeneric.end(),
        compat_set.begin(),
        compat_set.end(),
        std::back_inserter(result)
    );
    return result.empty();
}
