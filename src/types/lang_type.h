
#pragma once

#include <set>
#include <vector>

enum class LPrim { Invalid, Unit, Int, Bool, Char, Str };
enum class LTypeClass { Plus, Minus, Star, Slash, Percent, Eq, Ord };
typedef std::set<LTypeClass> LGeneric;
struct LArray;

typedef std::variant<LPrim, LGeneric, LArray> LTypeImpl;
typedef std::shared_ptr<LTypeImpl> LType;
typedef std::shared_ptr<LType> PtrLType;

struct LArray {
    PtrLType ltype;
    bool operator==(const LArray& other) const = default;
};

class TypeError {
  public:
    const LTypeImpl t1;
    const LTypeImpl t2;

    TypeError(const LTypeImpl& t1, const LTypeImpl& t2);
};

PtrLType make_lt(LPrim lprim);
PtrLType make_lt(LTypeClass tc);
PtrLType make_lt(LGeneric tcs);
PtrLType make_lt(LArray larray);
PtrLType make_lt(LTypeImpl ltypeimpl);

std::string to_string(LPrim lprim);
std::string to_string(LTypeClass tc);
std::string to_string(LGeneric tcs);
std::string to_string(LArray larray);

std::string to_string(LTypeImpl ltypeimpl);
std::string to_string(LType ltype);
std::string to_string(PtrLType ptrltype);
