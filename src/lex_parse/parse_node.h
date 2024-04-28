
#pragma once

#include <stddef.h>

#include <string>
#include <vector>

#include "state.h"

struct ParseNode {
    State state;
    std::string lexeme;
    std::vector<ParseNode> children;
    size_t line_no = 0;
    std::vector<State> get_production();
    std::string to_string(int depth);
};
