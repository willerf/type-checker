#pragma once

#include "ast_node.h"
#include "parse_node.h"

std::vector<std::shared_ptr<ASTNode>> extract_fns(ParseNode root);
std::shared_ptr<ASTNode> extract_fn(ParseNode root);
