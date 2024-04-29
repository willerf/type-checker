
#pragma once

#include <vector>
#include "ast_node.h"
#include "parse_node.h"

std::vector<std::shared_ptr<ASTNode>> extract_s(
    ParseNode root
);

