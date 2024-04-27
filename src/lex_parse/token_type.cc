
#include "token_type.h"

std::string to_string(TokenType token_type) {
    switch (token_type) {
        case TokenType::Newline:
            return "Newline";
    }
}
