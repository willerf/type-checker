
#pragma once

#include <vector>

enum class LPrim { Invalid, Int, Bool, Generic };
enum class LTypeClass { Plus, Minus, Star, Slash, Percent, Eq, Ord };
struct LCustom;

typedef std::variant<LPrim, std::vector<LTypeClass>, LCustom> LType;
typedef std::shared_ptr<LType> PtrLType;

struct LCustom {
    std::string name;
    std::vector<PtrLType> children;
    bool operator==(const LCustom& other) const = default;
};

PtrLType make_lt(LPrim lprim);
PtrLType make_lt(LTypeClass tc);

std::string to_string(LPrim lprim);
std::string to_string(LTypeClass tc);
std::string to_string(LCustom lcustom);

std::string to_string(LType ltype);
std::string to_string(PtrLType ptrltype);

