
#include "var_access_node.h"

VarAccessNode::VarAccessNode(Variable var) : var{var} {}

std::shared_ptr<VarAccessNode> make_var_access(Variable var) {
    return std::make_shared<VarAccessNode>(var);
}
