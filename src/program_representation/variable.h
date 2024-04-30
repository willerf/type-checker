
#pragma once

#include <string>
#include <vector>

#include "lang_type.h"

struct VarImpl {
    std::string name;
    std::variant<LPrim, LCustom> type_info;

    explicit VarImpl(std::string name, std::variant<LPrim, LCustom> type_info);
};

struct Variable {
    std::shared_ptr<VarImpl> impl;
    explicit Variable(const std::string& name = "", std::variant<LPrim, LCustom> type_info = LPrim::Generic);
};
