
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "lang_type.h"
#include "variable.h"

struct FnNode: ASTVisit<FnNode> {
    std::string name;
    std::vector<Variable> params;
    std::shared_ptr<ASTNode> stmts;
    PtrLType ret_type;
    explicit FnNode(
        std::string name,
        std::vector<Variable> params,
        std::shared_ptr<ASTNode> stmts,
        PtrLType ret_type
    );
};

std::shared_ptr<FnNode> make_fn(
    std::string name,
    std::vector<Variable> params,
    std::shared_ptr<ASTNode> stmts,
    LTypeImpl ret_type
);
