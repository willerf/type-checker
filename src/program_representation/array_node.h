
#pragma once

#include "ast_visit.h"

struct ArrayNode: ASTVisit<ArrayNode> {
    std::vector<std::shared_ptr<ASTNode>> init_list;
    std::shared_ptr<ASTNode> init_size;
    explicit ArrayNode(
        std::vector<std::shared_ptr<ASTNode>> init_list,
        std::shared_ptr<ASTNode> init_size
    );
};

std::shared_ptr<ArrayNode> make_array(
    std::vector<std::shared_ptr<ASTNode>> init_list,
    std::shared_ptr<ASTNode> init_size
);
