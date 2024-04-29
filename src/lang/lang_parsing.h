
#include <span>
#include <string_view>
#include <vector>

#include "dfa.h"
#include "grammar.h"
#include "parse_node.h"
#include "token.h"

struct Token;

Grammar make_lang_grammar();
ParseNode parse(std::span<Token> input);
