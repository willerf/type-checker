
#pragma once

#include <string>
#include <vector>

#include "lang_type.h"

struct VarImpl {
    std::string name;
    std::shared_ptr<LType> type_info;

    explicit VarImpl(const std::string& name, std::shared_ptr<LType> type_info);
};

struct Variable {
    std::shared_ptr<VarImpl> impl;
    explicit Variable(
        const std::string& name = "",
        LType type_info = LPrim::Generic
    );
};
