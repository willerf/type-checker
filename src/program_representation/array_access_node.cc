
#include "array_access_node.h"

ArrayAccessNode::ArrayAccessNode(
    std::shared_ptr<ASTNode> access_target,
    std::shared_ptr<ASTNode> index
) :
    access_target {access_target},
    index {index} {}

std::shared_ptr<ArrayAccessNode> make_array_access(
    std::shared_ptr<ASTNode> access_target,
    std::shared_ptr<ASTNode> index
) {
    return std::make_shared<ArrayAccessNode>(access_target, index);
}
