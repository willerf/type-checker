#include "array_node.h"

ArrayNode::ArrayNode(
    std::vector<std::shared_ptr<ASTNode>> init_list,
    std::shared_ptr<ASTNode> init_size
) :
    init_list {init_list},
    init_size {init_size} {}

std::shared_ptr<ArrayNode> make_array(
    std::vector<std::shared_ptr<ASTNode>> init_list,
    std::shared_ptr<ASTNode> init_size
) {
    return std::make_shared<ArrayNode>(init_list, init_size);
}
