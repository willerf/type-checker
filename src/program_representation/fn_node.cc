
#include "fn_node.h"

#include "lang_type.h"

FnNode::FnNode(
    std::string name,
    std::vector<Variable> params,
    std::shared_ptr<ASTNode> stmts,
    PtrLType ret_type
) :
    name {name},
    params {params},
    stmts {stmts},
    ret_type {ret_type} {}

std::shared_ptr<FnNode> make_fn(
    std::string name,
    std::vector<Variable> params,
    std::shared_ptr<ASTNode> stmts,
    LTypeImpl ret_type
) {
    return std::make_shared<FnNode>(name, params, stmts, make_lt(ret_type));
}
