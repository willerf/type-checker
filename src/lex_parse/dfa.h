
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>

#include "token_type.h"

struct DFA {
    std::set<char> alphabet;
    TokenType init_state;
    std::set<TokenType> valid_states;
    std::set<TokenType> accepting;
    std::function<std::optional<TokenType>(TokenType, char)> transition;
};

