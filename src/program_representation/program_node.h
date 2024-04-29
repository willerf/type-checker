
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"

struct ProgramNode: ASTVisit<ProgramNode> {
    std::vector<std::shared_ptr<ASTNode>> fns;
    explicit ProgramNode(std::vector<std::shared_ptr<ASTNode>> fns);
};

std::shared_ptr<ProgramNode> make_program(std::vector<std::shared_ptr<ASTNode>> fns);

