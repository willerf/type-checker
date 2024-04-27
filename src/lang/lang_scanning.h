
#include <string_view>
#include <vector>

#include "dfa.h"
#include "token.h"

DFA make_lang_dfa();
std::vector<Token> scan(std::string_view input);
