
#include "fn_node.h"

FnNode::FnNode(std::string name, std::vector<Variable> params, std::shared_ptr<ASTNode> stmts, std::shared_ptr<LType> ret_type) : name{name}, params{params}, stmts{stmts}, ret_type{ret_type} {}

std::shared_ptr<FnNode> make_fn(std::string name, std::vector<Variable> params, std::shared_ptr<ASTNode> stmts, std::shared_ptr<LType> ret_type) {
    return std::make_shared<FnNode>(name, params, stmts, ret_type);
}
