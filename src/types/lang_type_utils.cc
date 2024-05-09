
#include "lang_type_utils.h"

bool compatible(LPrim lprim, LGeneric lgeneric) {
    if (lgeneric.empty()) {
        return true;
    }

    LGeneric compat_set;
    std::vector<LTypeClass> result;
    switch (lprim) {
        case LPrim::Invalid:
            return false;
        case LPrim::Int:
            compat_set = {LTypeClass::Eq, LTypeClass::Ord, LTypeClass::Plus, LTypeClass::Minus, LTypeClass::Star, LTypeClass::Slash, LTypeClass::Percent};
            std::set_intersection(compat_set.begin(), compat_set.end(), lgeneric.begin(), lgeneric.end(), std::back_inserter(result));
            return !result.empty();
        case LPrim::Bool:
            compat_set = {LTypeClass::Eq};
            std::set_intersection(compat_set.begin(), compat_set.end(), lgeneric.begin(), lgeneric.end(), std::back_inserter(result));
            return !result.empty();
    }
}
