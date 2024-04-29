
#include "visitor.h"

#include <memory>
#include <vector>

#include "assign_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "literal_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"

template<>
void Visitor<void>::visit(std::shared_ptr<ASTNode> node) {}

template<>
void Visitor<void>::visit(std::shared_ptr<AssignNode> node) {
    node->rhs->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<BinaryExprNode> node) {
    node->lhs->accept(*this);
    node->rhs->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<CallNode> node) {
    for (auto arg : node->args) {
        arg->accept(*this);
    }
}

template<>
void Visitor<void>::visit(std::shared_ptr<FnNode> node) {
    node->stmts->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<IfNode> node) {
    node->condition->accept(*this);
    node->thens->accept(*this);
    if (node->elses) {
        node->elses->accept(*this);
    }
}

template<>
void Visitor<void>::visit(std::shared_ptr<LiteralNode> node) {
}

template<>
void Visitor<void>::visit(std::shared_ptr<RetNode> node) {
    node->expr->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<StmtBlockNode> node) {
    for (auto stmt : node->stmts) {
        stmt->accept(*this);
    }
}

template<>
void Visitor<void>::visit(std::shared_ptr<UnaryExprNode> node) {
    node->expr->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<VarAccessNode> node) {
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<ASTNode> node) {
    return node;
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<AssignNode> node) {
    auto rhs = node->rhs->accept(*this);
    return make_assign(node->lhs, rhs);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    return make_binary_expr(lhs, node->op, rhs);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<CallNode> node) {
    std::vector<std::shared_ptr<ASTNode>> args;
    for (auto arg : node->args) {
        args.push_back(arg->accept(*this));
    }
    return make_call(node->proc_name, args);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<FnNode> node) {
    auto result = node->stmts->accept(*this); 
    return make_fn(node->name, node->params, result);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<IfNode> node) {
    auto condition = node->condition->accept(*this);
    auto thens = node->thens->accept(*this);
    std::shared_ptr<ASTNode> elses = nullptr;
    if (node->elses) {
        elses = node->elses->accept(*this);
    }
    return make_if(condition, thens, elses);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<LiteralNode> node) {
    return std::static_pointer_cast<ASTNode>(node);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<RetNode> node) {
    auto expr = node->expr->accept(*this);
    return make_ret(expr);
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
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr = node->expr->accept(*this);
    return make_unary_expr(node->op, expr);
}

template<>
std::shared_ptr<ASTNode>
Visitor<std::shared_ptr<ASTNode>>::visit(std::shared_ptr<VarAccessNode> node) {
    return std::static_pointer_cast<ASTNode>(node);
}

