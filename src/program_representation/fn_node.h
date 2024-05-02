
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct FnNode: ASTVisit<FnNode> {
    std::string name;
    std::vector<Variable> params;
    std::shared_ptr<ASTNode> stmts;
    std::shared_ptr<LType> ret_type;
    explicit FnNode(
        std::string name,
        std::vector<Variable> params,
        std::shared_ptr<ASTNode> stmts,
        std::shared_ptr<LType> ret_type
    );
};

std::shared_ptr<FnNode> make_fn(
    std::string name,
    std::vector<Variable> params,
    std::shared_ptr<ASTNode> stmts,
    std::shared_ptr<LType> ret_type
);
