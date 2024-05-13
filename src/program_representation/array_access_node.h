
#pragma once

#include "ast_visit.h"

struct ArrayAccessNode: ASTVisit<ArrayAccessNode> {
    std::shared_ptr<ASTNode> access_target;
    std::shared_ptr<ASTNode> index;
    explicit ArrayAccessNode(
        std::shared_ptr<ASTNode> access_target,
        std::shared_ptr<ASTNode> index
    );
};

std::shared_ptr<ArrayAccessNode> make_array_access(
    std::shared_ptr<ASTNode> access_target,
    std::shared_ptr<ASTNode> index
);
