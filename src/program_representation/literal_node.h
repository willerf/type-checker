

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"

enum class LiteralType { Int, Bool };

struct LiteralNode: ASTVisit<LiteralNode> {
    LiteralType literal_type;
    std::string value;

    explicit LiteralNode(LiteralType literal_type, std::string value);
};

std::shared_ptr<LiteralNode>
make_literal(LiteralType literal_type, std::string value);
