
#include "ret_node.h"

RetNode::RetNode(std::shared_ptr<ASTNode> expr) : expr{expr} {}

std::shared_ptr<ASTNode> ret_node(std::shared_ptr<ASTNode> expr) {
    return std::make_shared<RetNode>(expr);
}
