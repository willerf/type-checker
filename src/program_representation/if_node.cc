
#include "if_node.h"

IfNode::IfNode(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> thens, std::shared_ptr<ASTNode> elses) : condition{condition}, thens{thens}, elses{elses} {}

std::shared_ptr<IfNode> make_if(std::shared_ptr<ASTNode> condition, std::shared_ptr<ASTNode> thens, std::shared_ptr<ASTNode> elses) {
    return std::make_shared<IfNode>(condition, thens, elses);
}
