
#pragma once

#include "lang_type.h"

template<class... Ts>
struct overloaded: Ts... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

bool compatible(LPrim lprim, LGeneric lgeneric);
bool compatible(LArray larray, LGeneric lgeneric);
