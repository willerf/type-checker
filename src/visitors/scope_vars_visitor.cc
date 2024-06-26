
#include "scope_vars_visitor.h"

#include <cassert>
#include <iostream>

#include "array_access_node.h"
#include "array_node.h"
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
#include "var_decl_node.h"
#include "visitor.h"
#include "while_node.h"

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<ArrayAccessNode> node) {
    auto access_target = node->access_target->accept(*this);
    auto index = node->index->accept(*this);

    return make_array_access(access_target, index);
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<ArrayNode> node) {
    std::vector<std::shared_ptr<ASTNode>> init_list;
    for (auto init_val : node->init_list) {
        init_list.push_back(init_val->accept(*this));
    }
    std::shared_ptr<ASTNode> init_size;
    if (node->init_size) {
        init_size = node->init_size->accept(*this);
    }
    return make_array(init_list, init_size);
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    return make_assign(lhs, rhs);
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<FnNode> node
) {
    scopes = {};

    std::map<std::string, Variable> scope;
    std::vector<Variable> params;
    for (auto param : node->params) {
        Variable v(param.impl->name);
        params.push_back(v);
        scope[v.impl->name] = v;
    }
    scopes.push(scope);
    auto result =
        make_fn(node->name, params, node->stmts->accept(*this), LGeneric {});
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
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

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    auto result = make_binary_expr(lhs, node->op, rhs);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<CallNode> node
) {
    std::vector<std::shared_ptr<ASTNode>> args;
    for (auto arg : node->args) {
        args.push_back(arg->accept(*this));
    }
    auto result = make_call(node->func_name, args);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<IfNode> node
) {
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

std::shared_ptr<ASTNode> ScopedVarsVisitor::visit(std::shared_ptr<RetNode> node
) {
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

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    auto scope = scopes.top();
    if (!scope.contains(node->var.impl->name)) {
        throw VariableNotFoundError(node->var.impl->name, node->line_no);
    }
    auto var = scope[node->var.impl->name];
    auto result = make_var_access(var);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<VarDeclNode> node) {
    auto name = node->var.impl->name;
    auto rhs = node->rhs->accept(*this);
    Variable var(name);
    scopes.top()[name] = var;
    auto result = make_var_decl(var, rhs);
    result->line_no = node->line_no;
    return result;
}

std::shared_ptr<ASTNode>
ScopedVarsVisitor::visit(std::shared_ptr<WhileNode> node) {
    auto condition = node->condition->accept(*this);
    auto body = node->body->accept(*this);
    auto result = make_while(condition, body);
    result->line_no = node->line_no;
    return result;
}

VariableNotFoundError::VariableNotFoundError(
    const std::string& name,
    size_t line_no
) :
    name {name},
    line_no {line_no} {}
