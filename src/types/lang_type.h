
#pragma once

#include <vector>

enum class LPrim {
    Int,
    Bool,
    Generic
};

std::string to_string(LPrim lprim);

struct LCustom {
    // std::vector<std::variant<LPrim, LCustom>> children;
    auto operator<=>(const LCustom& other) const = default;
};

std::string to_string(std::variant<LPrim, LCustom> ltype);
