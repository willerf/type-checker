
#include "binary_expr_node.h"
#include <memory>

BinaryExprNode::BinaryExprNode(std::shared_ptr<ASTNode> lhs, BinaryOp op, std::shared_ptr<ASTNode> rhs) : lhs{lhs}, op{op}, rhs{rhs} {}

std::shared_ptr<BinaryExprNode> make_binary_expr(std::shared_ptr<ASTNode> lhs, BinaryOp op, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<BinaryExprNode>(lhs, op, rhs);
}
