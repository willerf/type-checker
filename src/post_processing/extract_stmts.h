
#pragma once

#include <memory>
#include <vector>
#include "ast_node.h"
#include "parse_node.h"

std::shared_ptr<ASTNode> extract_stmt(
    ParseNode root
);

std::vector<std::shared_ptr<ASTNode>> extract_stmts(
    ParseNode root
);

std::shared_ptr<ASTNode> extract_stmtblock(
    ParseNode root
);
