
#pragma once

#include <vector>
#include <set>

enum class LPrim { Invalid, Int, Bool };
enum class LTypeClass { Plus, Minus, Star, Slash, Percent, Eq, Ord };
typedef std::set<LTypeClass> LGeneric;
struct LCustom;

typedef std::variant<LPrim, LGeneric, LCustom> LTypeImpl;
typedef std::shared_ptr<LTypeImpl> LType;
typedef std::shared_ptr<LType> PtrLType;

struct LCustom {
    std::string name;
    std::vector<PtrLType> children;
    bool operator==(const LCustom& other) const = default;
};

PtrLType make_lt(LPrim lprim);
PtrLType make_lt(LTypeClass tc);
PtrLType make_lt(LGeneric tcs);
PtrLType make_lt(LCustom lcustom);
PtrLType make_lt(LTypeImpl ltypeimpl);

std::string to_string(LPrim lprim);
std::string to_string(LTypeClass tc);
std::string to_string(LGeneric tcs);
std::string to_string(LCustom lcustom);

std::string to_string(LTypeImpl ltypeimpl);
std::string to_string(LType ltype);
std::string to_string(PtrLType ptrltype);

bool compatible(LPrim lprim, LGeneric lgeneric);
