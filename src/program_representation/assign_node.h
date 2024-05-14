

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct AssignNode: ASTVisit<AssignNode> {
    std::shared_ptr<ASTNode> lhs;
    std::shared_ptr<ASTNode> rhs;
    explicit AssignNode(
        std::shared_ptr<ASTNode> lhs,
        std::shared_ptr<ASTNode> rhs
    );
};

std::shared_ptr<AssignNode>
make_assign(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs);
