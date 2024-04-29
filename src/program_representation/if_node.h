
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "stmt_block_node.h"
#include "variable.h"

struct IfNode: ASTVisit<IfNode> {
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> thens;
    std::shared_ptr<ASTNode> elses;
    explicit IfNode(
        std::shared_ptr<ASTNode> condition,
        std::shared_ptr<ASTNode> thens,
        std::shared_ptr<ASTNode> elses
    );
};

std::shared_ptr<IfNode> make_if(
    std::shared_ptr<ASTNode> condition,
    std::shared_ptr<ASTNode> thens,
    std::shared_ptr<ASTNode> elses
);
