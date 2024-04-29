
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"
#include "stmt_block_node.h"

struct FnNode: ASTVisit<FnNode> {
    std::vector<Variable> params;
    std::shared_ptr<ASTNode> stmts;
    explicit FnNode(std::vector<Variable> params, std::shared_ptr<ASTNode> stmts);
};

std::shared_ptr<FnNode> make_fn(std::vector<Variable> params, std::shared_ptr<ASTNode> stmts);
