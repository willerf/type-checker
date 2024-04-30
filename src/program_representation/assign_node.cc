
#include "assign_node.h"

AssignNode::AssignNode(bool declaration, Variable lhs, std::shared_ptr<ASTNode> rhs) : declaration{declaration}, lhs{lhs}, rhs{rhs} {}

std::shared_ptr<AssignNode> make_assign(bool declaration, Variable lhs, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<AssignNode>(declaration, lhs, rhs);
}
