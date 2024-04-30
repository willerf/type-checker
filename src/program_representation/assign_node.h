

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct AssignNode: ASTVisit<AssignNode> {
    bool declaration;
    Variable lhs;
    std::shared_ptr<ASTNode> rhs;
    explicit AssignNode(bool declaration, Variable lhs, std::shared_ptr<ASTNode> rhs);
};

std::shared_ptr<AssignNode>
make_assign(bool declaration, Variable lhs, std::shared_ptr<ASTNode> rhs);
