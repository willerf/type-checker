
#include "ret_node.h"

RetNode::RetNode(std::shared_ptr<ASTNode> expr) : expr{expr} {}

std::shared_ptr<ASTNode> make_ret(std::shared_ptr<ASTNode> expr) {
    return std::make_shared<RetNode>(expr);
}
