
#include "unary_expr_node.h"
#include <memory>

UnaryExprNode::UnaryExprNode(UnaryOp op, std::shared_ptr<ASTNode> expr) : op{op}, expr{expr} {}

std::shared_ptr<UnaryExprNode> make_unary_expr(UnaryOp op, std::shared_ptr<ASTNode> expr) {
    return std::make_shared<UnaryExprNode>(op, expr);
}

std::string to_string(UnaryOp op) {
    switch (op) {
        case UnaryOp::NOT:
            return "!";
    }
}

