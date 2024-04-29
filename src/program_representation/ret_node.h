
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"
#include "variable.h"

struct RetNode: ASTVisit<RetNode> {
    std::shared_ptr<ASTNode> expr;
    explicit RetNode(std::shared_ptr<ASTNode> expr);
};

std::shared_ptr<ASTNode> make_ret(std::shared_ptr<ASTNode> expr);
