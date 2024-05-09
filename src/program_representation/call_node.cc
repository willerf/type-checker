
#include "call_node.h"

CallNode::CallNode(
    std::string func_name,
    std::vector<std::shared_ptr<ASTNode>> args
) :
    func_name {func_name},
    args {args} {}

std::shared_ptr<CallNode>
make_call(std::string func_name, std::vector<std::shared_ptr<ASTNode>> args) {
    return std::make_shared<CallNode>(func_name, args);
}
