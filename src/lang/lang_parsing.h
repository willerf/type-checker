
#include <span>
#include <string_view>
#include <vector>

#include "ast_node.h"
#include "dfa.h"
#include "grammar.h"
#include "token.h"

struct Token;

Grammar make_lang_grammar();
ParseNode parse(std::span<Token> input);

