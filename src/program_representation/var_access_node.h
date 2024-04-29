

#pragma once

#include "ast_visit.h"
#include "variable.h"

struct VarAccessNode: ASTVisit<VarAccessNode> {
    Variable var;
    explicit VarAccessNode(Variable var);
};

std::shared_ptr<VarAccessNode> make_var_access(Variable var);
