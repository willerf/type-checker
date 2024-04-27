
#pragma once

#include <ostream>
#include <string>

#include "token_type.h"

struct Token {
    TokenType kind;
    std::string lexeme;
    size_t line_no = 0;

    bool operator==(const Token&) const;
};

std::ostream& operator<<(std::ostream&, const Token&);
