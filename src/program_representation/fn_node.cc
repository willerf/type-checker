
#include "fn_node.h"

FnNode::FnNode(std::vector<Variable> params, std::shared_ptr<ASTNode> stmts) : params{params}, stmts{stmts} {}

std::shared_ptr<FnNode> make_fn(std::vector<Variable> params, std::shared_ptr<ASTNode> stmts) {
    return std::make_shared<FnNode>(params, stmts);
}
