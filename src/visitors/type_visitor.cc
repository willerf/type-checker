
#include "type_visitor.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "lang_type.h"
#include "literal_node.h"
#include "program_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"

#include <iostream>
#include <cassert>
#include <memory>

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<ASTNode> node) {
    std::cerr << "TypeVisitor error" << std::endl;
    return std::variant<LPrim, LCustom>();
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto expr_type_info = node->rhs->accept(*this);
    if (node->declaration) {
        node->lhs.impl->type_info = expr_type_info; 
    }
    else {
        if (node->lhs.impl->type_info != expr_type_info) {
            std::cerr << "Assignment error on line " << node->line_no << std::endl;
            std::cerr << "Attempting to assign " << to_string(expr_type_info) << " to " << to_string(node->lhs.impl->type_info) << std::endl;
            exit(1);
        }
    }
    return LPrim::Generic;
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    if (std::holds_alternative<LPrim>(lhs) && std::get<LPrim>(lhs) == LPrim::Generic) {
        lhs = rhs;
    }
    if (std::holds_alternative<LPrim>(rhs) && std::get<LPrim>(rhs) == LPrim::Generic) {
        rhs = lhs;
    }
    if (lhs != rhs) {
        std::cerr << "Binary expression error on line " << node->line_no << std::endl;
        std::cerr << "Invalid use of operator: " << to_string(lhs) << " " << to_string(node->op) << " " << to_string(rhs) << std::endl;
        exit(1);
    }
    static std::set<BinaryOp> s1 = {BinaryOp::EQ, BinaryOp::GE};
    static std::set<BinaryOp> s2 = {BinaryOp::GT, BinaryOp::LE, BinaryOp::LT, BinaryOp::NE};
    static std::set<BinaryOp> s3 = {BinaryOp::PLUS, BinaryOp::MINUS, BinaryOp::TIMES, BinaryOp::DIVIDE, BinaryOp::MOD};
    static std::set<BinaryOp> s4 = {BinaryOp::OR, BinaryOp::AND};
    switch (std::get<LPrim>(lhs)) {
        case LPrim::Int:
            if (s1.contains(node->op) || s2.contains(node->op)) {
                return LPrim::Bool;
            }
            if (s3.contains(node->op)) {
                return LPrim::Int;
            }
            std::cerr << "Binary expression error on line " << node->line_no << std::endl;
            std::cerr << "Invalid use of operator: " << to_string(lhs) << " " << to_string(node->op) << " " << to_string(rhs) << std::endl;
            exit(1);
        case LPrim::Bool:
            if (s1.contains(node->op) || s4.contains(node->op)) {
                return LPrim::Bool;
            }
            std::cerr << "Binary expression error on line " << node->line_no << std::endl;
            std::cerr << "Invalid use of operator: " << to_string(lhs) << " " << to_string(node->op) << " " << to_string(rhs) << std::endl;
            exit(1);
        case LPrim::Generic:
            if (s1.contains(node->op) || s2.contains(node->op) || s4.contains(node->op)) {
                return LPrim::Bool;
            }
            if (s3.contains(node->op)) {
                return LPrim::Int;
            }
        default:
            std::cerr << "BinaryExpr error" << std::endl;
            exit(1);
    }
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<CallNode> node) {
    std::vector<std::variant<LPrim, LCustom>> args;
    for (auto expr : node->args) {
        args.push_back(expr->accept(*this));
    }
    if (!m.contains(node->proc_name)) {
        std::cerr << "Invalid function call on line " << node->line_no << std::endl;
        exit(1);
    }
    auto fn = m[node->proc_name];
    if (completed.contains({fn, args})) {
        return completed[{fn, args}];
    }
    else {
        if (args.size() != fn->params.size()) {
            std::cerr << "Invalid function call on line " << node->line_no << std::endl;
            exit(1);
        }
        for (int i = 0; i < args.size(); i++) {
            fn->params[i].impl->type_info = args[i];
        }
        completed[{fn, args}] = LPrim::Generic;
        auto result = fn->accept(*this);
        completed[{fn, args}] = result;
        return result;
    }
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<FnNode> node) {
    return node->stmts->accept(*this);
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<IfNode> node) {
    auto condition = node->condition->accept(*this);
    if (!(std::holds_alternative<LPrim>(condition) && std::get<LPrim>(condition) == LPrim::Bool)) {
        std::cerr << "Invalid condition on line " << node->line_no << std::endl;
        std::cerr << "Expected bool, found " << to_string(condition) << std::endl;
        exit(1);
    }
    auto thens = node->thens->accept(*this);
    auto elses = node->elses->accept(*this);
    if (std::holds_alternative<LPrim>(thens) && std::get<LPrim>(thens) == LPrim::Generic) {
        return elses;
    }
    if (std::holds_alternative<LPrim>(elses) && std::get<LPrim>(elses) == LPrim::Generic) {
        return thens;
    }
    if (thens == elses) {
        return thens;
    }
    std::cerr << "Inconsistent return types found in if else on line " << node->line_no << std::endl;
    std::cerr << "True branch returning " << to_string(thens) << ", false branch returning " << to_string(elses) << std::endl;
    exit(1);
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<LiteralNode> node) {
    switch (node->literal_type) {
        case LiteralType::Bool:
            return LPrim::Bool;
        case LiteralType::Int:
            return LPrim::Int;
    }
    std::cerr << "Literal error" << std::endl;
    exit(1);
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<ProgramNode> node) {
    m["print"] = make_fn("print", {Variable("var")}, make_ret(make_literal(LiteralType::Bool, "true")));
    m["println"] = make_fn("println", {Variable("var")}, make_ret(make_literal(LiteralType::Bool, "true")));
    for (auto node : node->fns) {
        auto fn = std::dynamic_pointer_cast<FnNode>(node);
        if (fn) {
            m[fn->name] = fn;
        }
        else {
            std::cerr << "Program error" << std::endl;
            exit(1);
        }
    }
    if (m.contains("main")) {
        m["main"]->accept(*this);
    }
    else {
        std::cerr << "Missing main function" << std::endl;
        exit(1);
    }
    return LPrim::Generic;
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<RetNode> node) {
    return node->expr->accept(*this);
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    std::variant<LPrim, LCustom> result = LPrim::Generic;
    size_t line_no = SIZE_T_MAX;
    for (auto stmt : node->stmts) {
        auto stmt_type = stmt->accept(*this); 
        if (!(std::holds_alternative<LPrim>(stmt_type) && std::get<LPrim>(stmt_type) == LPrim::Generic)) {
            if (std::holds_alternative<LPrim>(result) && std::get<LPrim>(result) == LPrim::Generic) {
                result = stmt_type;
                line_no = stmt->line_no;
            }
            else {
                if (result != stmt_type) {
                    std::cerr << "Inconsistent return types found" << std::endl;
                    std::cerr << "Line " << line_no << ": " << to_string(result);
                    std::cerr << "Line " << stmt->line_no << ": " << to_string(stmt_type);
                    exit(1);
                }
            }
        }
    }
    return result;
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr = node->expr->accept(*this);
    switch (node->op) {
        case UnaryOp::NOT:
            if (std::holds_alternative<LPrim>(expr) && std::get<LPrim>(expr) == LPrim::Bool) {
                return LPrim::Bool;
            }
            std::cerr << "Unary expression error on line " << node->line_no << std::endl;
            std::cerr << "Invalid use of operator: " << to_string(node->op) << " " << to_string(expr) << std::endl;
            exit(1);
    }
}

std::variant<LPrim, LCustom> TypeVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    return node->var.impl->type_info;
}

