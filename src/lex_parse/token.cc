
#include "token.h"

#include <ostream>

bool Token::operator==(const Token& other) const {
    return kind == other.kind && lexeme == other.lexeme;
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "(" << to_string(token.kind) << ", " << token.lexeme << ")";
    return out;
}

