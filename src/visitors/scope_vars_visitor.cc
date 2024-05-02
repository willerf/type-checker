
#include "scope_vars_visitor.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "literal_node.h"
#include "program_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"
#include "visitor.h"

#include <iostream>
#include <cassert>

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto name = node->lhs.impl->name;
    auto rhs = node->rhs->accept(*this);
    if (node->declaration) {
        Variable lhs(name, LPrim::Generic);
        scopes.top()[name] = lhs;
        auto result = make_assign(node->declaration, lhs, rhs);
        result->line_no = node->line_no;
        return result;
    }
    else {
        if (!scopes.top().contains(name)) {
            std::cerr << "Unknown variable " << name << " on line " << node->line_no << std::endl; 
            exit(1);
        }
        auto lhs = scopes.top()[name];
        auto result = make_assign(node->declaration, lhs, rhs);
        result->line_no = node->line_no;
        return result;
    }
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<FnNode> node) {
    scopes = {};
     
    std::map<std::string, Variable> scope;
    std::vector<Variable> params;
    for (auto param : node->params) {
        Variable v(param.impl->name, LPrim::Generic);
        params.push_back(v);
        scope[v.impl->name] = v;
    }
    scopes.push(scope);
    auto result = make_fn(node->name, params, node->stmts->accept(*this));
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    auto scope = scopes.top();
    scopes.push(scope);

    std::vector<std::shared_ptr<ASTNode>> stmts;
    for (auto stmt : node->stmts) {
        stmts.push_back(stmt->accept(*this)); 
    }

    scopes.pop();
    auto result = make_stmt_block(stmts);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    auto var = scopes.top()[node->var.impl->name];
    auto result = make_var_access(var);
    result->line_no = node->line_no;
    return result;
}


std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<ASTNode> node) {
    return node;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    auto result = make_binary_expr(lhs, node->op, rhs);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<CallNode> node) {
    std::vector<std::shared_ptr<ASTNode>> args;
    for (auto arg : node->args) {
        args.push_back(arg->accept(*this));
    }
    auto result = make_call(node->proc_name, args);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<IfNode> node) {
    auto condition = node->condition->accept(*this);
    auto thens = node->thens->accept(*this);
    std::shared_ptr<ASTNode> elses = nullptr;
    if (node->elses) {
        elses = node->elses->accept(*this);
    }
    auto result = make_if(condition, thens, elses);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<LiteralNode> node) {
    auto result = make_literal(node->literal_type, node->value);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<ProgramNode> node) {
    std::vector<std::shared_ptr<ASTNode>> fns;
    for (auto fn : node->fns) {
        fns.push_back(fn->accept(*this));
    }
    auto result = make_program(fns);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<RetNode> node) {
    auto expr = node->expr->accept(*this);
    auto result = make_ret(expr);
    result->line_no = node->line_no;
    return result;
}



std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr = node->expr->accept(*this);
    auto result = make_unary_expr(node->op, expr);
    result->line_no = node->line_no;
    return result;
}

