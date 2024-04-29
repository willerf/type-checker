
#include "assign_node.h"

AssignNode::AssignNode(Variable lhs, std::shared_ptr<ASTNode> rhs) : lhs{lhs}, rhs{rhs} {}

std::shared_ptr<AssignNode> make_assign(Variable lhs, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<AssignNode>(lhs, rhs);
}
