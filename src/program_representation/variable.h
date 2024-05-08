
#pragma once

#include <string>
#include <vector>

#include "lang_type.h"

struct VarImpl {
    std::string name;
    PtrLType ptr_ltype;

    explicit VarImpl(const std::string& name,  PtrLType ptr_ltype);
};

struct Variable {
    std::shared_ptr<VarImpl> impl;
    explicit Variable(
        const std::string& name = "",
        LTypeImpl ltypeimpl = LPrim::Generic
    );
};
