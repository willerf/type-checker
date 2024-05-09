

#pragma once

#include "ast_visit.h"

struct CallNode: ASTVisit<CallNode> {
    std::string func_name;
    std::vector<std::shared_ptr<ASTNode>> args;
    explicit CallNode(
        std::string func_name,
        std::vector<std::shared_ptr<ASTNode>> args
    );
};

std::shared_ptr<CallNode>
make_call(std::string func_name, std::vector<std::shared_ptr<ASTNode>> args);
