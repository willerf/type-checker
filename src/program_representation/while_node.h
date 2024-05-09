
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "stmt_block_node.h"
#include "variable.h"

struct WhileNode: ASTVisit<WhileNode> {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    explicit WhileNode(
        std::shared_ptr<ASTNode> condition,
        std::shared_ptr<ASTNode> body
    );
};

std::shared_ptr<WhileNode>
make_while(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> body);
