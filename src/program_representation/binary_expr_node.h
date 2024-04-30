
#pragma once

#include <memory>
#include <vector>

#include "ast_visit.h"

enum class BinaryOp {
    OR,
    AND,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    MOD
};

std::string to_string(BinaryOp op);

struct BinaryExprNode: ASTVisit<BinaryExprNode> {
    std::shared_ptr<ASTNode> lhs;
    BinaryOp op;
    std::shared_ptr<ASTNode> rhs;
    explicit BinaryExprNode(
        std::shared_ptr<ASTNode> lhs,
        BinaryOp op,
        std::shared_ptr<ASTNode> rhs
    );
};

std::shared_ptr<BinaryExprNode> make_binary_expr(
    std::shared_ptr<ASTNode> lhs,
    BinaryOp op,
    std::shared_ptr<ASTNode> rhs
);
