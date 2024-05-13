
#include "var_decl_node.h"

VarDeclNode::VarDeclNode(Variable var, std::shared_ptr<ASTNode> rhs) :
    var {var},
    rhs {rhs} {}

std::shared_ptr<VarDeclNode>
make_var_decl(Variable var, std::shared_ptr<ASTNode> rhs) {
    return std::make_shared<VarDeclNode>(var, rhs);
}
