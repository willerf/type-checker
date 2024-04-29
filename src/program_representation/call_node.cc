
#include "call_node.h"

CallNode::CallNode(std::string proc_name, std::vector<std::shared_ptr<ASTNode>> args) : proc_name{proc_name}, args{args} {}

std::shared_ptr<CallNode> make_call(std::string proc_name, std::vector<std::shared_ptr<ASTNode>> args) {
    return std::make_shared<CallNode>(proc_name, args);
}
