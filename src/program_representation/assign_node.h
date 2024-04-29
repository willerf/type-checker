

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct AssignNode: ASTVisit<AssignNode> {
    Variable lhs;
    std::shared_ptr<ASTNode> rhs;
    explicit AssignNode(Variable lhs, std::shared_ptr<ASTNode> rhs);
};

std::shared_ptr<AssignNode>
make_assign(Variable lhs, std::shared_ptr<ASTNode> rhs);
