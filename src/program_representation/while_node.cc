
#include "while_node.h"

WhileNode::WhileNode(
    std::shared_ptr<ASTNode> condition,
    std::shared_ptr<ASTNode> body
) :
    condition {condition},
    body {body} {}

std::shared_ptr<WhileNode>
make_while(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> body) {
    return std::make_shared<WhileNode>(condition, body);
}
