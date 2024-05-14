
#include "assign_node.h"

AssignNode::AssignNode(
    std::shared_ptr<ASTNode> lhs,
    std::shared_ptr<ASTNode> rhs
) :
    lhs {lhs},
    rhs {rhs} {}

std::shared_ptr<AssignNode>
make_assign(std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<AssignNode>(lhs, rhs);
}
