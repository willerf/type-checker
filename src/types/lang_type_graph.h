#pragma once

#include <map>

#include "lang_type.h"
class LangTypeGraph {
    std::map<PtrLType, std::vector<PtrLType>> type_sets;
public:
    PtrLType union_types(PtrLType t1, PtrLType t2); 
    PtrLType add_tc(PtrLType lt, LTypeClass tc);
    PtrLType add_type(PtrLType lt);
};





