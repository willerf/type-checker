

#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"

enum class UnaryOp { NOT };

std::string to_string(UnaryOp op);

struct UnaryExprNode: ASTVisit<UnaryExprNode> {
    UnaryOp op;
    std::shared_ptr<ASTNode> expr;
    explicit UnaryExprNode(UnaryOp op, std::shared_ptr<ASTNode> expr);
};

std::shared_ptr<UnaryExprNode>
make_unary_expr(UnaryOp op, std::shared_ptr<ASTNode> expr);
