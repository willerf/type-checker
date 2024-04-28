

#pragma once

#include <optional>
#include <span>

#include "parse_node.h"
#include "grammar.h"
#include "token.h"

struct Grammar;
struct Token;

std::optional<ParseNode> parse_earley(std::span<Token> input, Grammar& grammar);
