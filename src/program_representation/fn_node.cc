
#include "fn_node.h"

FnNode::FnNode(std::string name, std::vector<Variable> params, std::shared_ptr<ASTNode> stmts) : name{name}, params{params}, stmts{stmts} {}

std::shared_ptr<FnNode> make_fn(std::string name, std::vector<Variable> params, std::shared_ptr<ASTNode> stmts) {
    return std::make_shared<FnNode>(name, params, stmts);
}
