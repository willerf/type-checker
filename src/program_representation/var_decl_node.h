

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct VarDeclNode: ASTVisit<VarDeclNode> {
    Variable var;
    std::shared_ptr<ASTNode> rhs;
    explicit VarDeclNode(Variable var, std::shared_ptr<ASTNode> rhs);
};

std::shared_ptr<VarDeclNode>
make_var_decl(Variable var, std::shared_ptr<ASTNode> rhs);
