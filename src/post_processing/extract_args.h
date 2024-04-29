
#pragma once

#include <memory>
#include "ast_node.h"
#include "parse_node.h"

std::vector<std::shared_ptr<ASTNode>> extract_args(
    ParseNode root
);

std::vector<std::shared_ptr<ASTNode>> extract_optargs(
    ParseNode root
);
