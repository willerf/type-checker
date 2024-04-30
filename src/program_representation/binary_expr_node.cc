
#include "binary_expr_node.h"
#include <memory>

BinaryExprNode::BinaryExprNode(std::shared_ptr<ASTNode> lhs, BinaryOp op, std::shared_ptr<ASTNode> rhs) : lhs{lhs}, op{op}, rhs{rhs} {}

std::shared_ptr<BinaryExprNode> make_binary_expr(std::shared_ptr<ASTNode> lhs, BinaryOp op, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<BinaryExprNode>(lhs, op, rhs);
}

std::string to_string(BinaryOp op) {
    switch (op) {
        case BinaryOp::OR:
            return "||";
        case BinaryOp::AND:
            return "&&";
        case BinaryOp::EQ:
            return "==";
        case BinaryOp::NE:
            return "!=";
        case BinaryOp::LT:
            return "<";
        case BinaryOp::GT:
            return ">";
        case BinaryOp::LE:
            return "<=";
        case BinaryOp::GE:
            return ">=";
        case BinaryOp::PLUS:
            return "+";
        case BinaryOp::MINUS:
            return "-";
        case BinaryOp::TIMES:
            return "*";
        case BinaryOp::DIVIDE:
            return "/";
        case BinaryOp::MOD:
            return "%";
    }
}
