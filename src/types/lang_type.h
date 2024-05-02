
#pragma once

#include <vector>

enum class LPrim { Invalid, Int, Bool, Generic };

std::string to_string(LPrim lprim);

struct LCustom {
    // std::vector<std::variant<LPrim, LCustom>> children;
    auto operator<=>(const LCustom& other) const = default;
};

typedef std::variant<LPrim, LCustom> LType;

std::string to_string(LType ltype);

