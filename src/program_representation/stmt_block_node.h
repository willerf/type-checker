#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"

struct StmtBlockNode: ASTVisit<StmtBlockNode> {
    std::vector<std::shared_ptr<ASTNode>> stmts;

    explicit StmtBlockNode(std::vector<std::shared_ptr<ASTNode>> stmts);
};

std::shared_ptr<StmtBlockNode>
make_stmt_block(std::vector<std::shared_ptr<ASTNode>> stmts);
