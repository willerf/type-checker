
#pragma once

#include <memory>

#include "ast_node.h"
#include "parse_node.h"

std::shared_ptr<ASTNode> extract_expr(ParseNode root);
