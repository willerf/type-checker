
#include "visitor.h"

#include <vector>

#include "stmt_block_node.h"
#include "fn_node.h"

template<>
void Visitor<void>::visit(std::shared_ptr<ASTNode> node) {}

template<>
void Visitor<void>::visit(std::shared_ptr<StmtBlockNode> node) {
    for (auto stmt : node->stmts) {
        stmt->accept(*this);
    }
}

template<>
void Visitor<void>::visit(std::shared_ptr<FnNode> node) {
    node->stmts->accept(*this);
}


template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<ASTNode> node) {
    return node;
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<StmtBlockNode> node) {
    std::vector<std::shared_ptr<ASTNode>> result;
    for (auto stmt : node->stmts) {
        result.push_back(stmt->accept(*this));
    }
    return make_stmt_block(result);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<FnNode> node) {
    auto result = node->stmts->accept(*this); 
    return make_fn(node->params, result);
}
