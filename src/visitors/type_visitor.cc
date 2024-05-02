
#include "type_visitor.h"

#include <cassert>
#include <iostream>
#include <memory>
#include "lang_type.h"
#include "unary_expr_node.h"

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<ASTNode> node) {
    std::cerr << "TypeVisitor error" << std::endl;
    exit(1);
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto expr_type = node->rhs->accept(*this);
    edges.push_back({node->lhs.impl->type_info, expr_type});
    edges.push_back({expr_type, node->lhs.impl->type_info});
    return std::make_shared<LType>(LPrim::Invalid);
}

std::pair<LPrim, LPrim> op_types(BinaryOp op) {
    switch (op) {
        case BinaryOp::OR:
        case BinaryOp::AND:
            return {LPrim::Bool, LPrim::Bool};
        case BinaryOp::EQ:
        case BinaryOp::NE:
            return {LPrim::Generic, LPrim::Bool};
        case BinaryOp::LT:
        case BinaryOp::GT:
        case BinaryOp::LE:
        case BinaryOp::GE:
            return {LPrim::Int, LPrim::Bool};
        case BinaryOp::PLUS:
        case BinaryOp::MINUS:
        case BinaryOp::TIMES:
        case BinaryOp::DIVIDE:
        case BinaryOp::MOD:
            return {LPrim::Int, LPrim::Int};
    }
}
std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs_type = node->lhs->accept(*this);
    auto rhs_type = node->rhs->accept(*this);
    edges.push_back({lhs_type, rhs_type});
    edges.push_back({rhs_type, lhs_type});

    auto in_out = op_types(node->op);
    auto input_type = std::make_shared<LType>(in_out.first);
    auto output_type = std::make_shared<LType>(in_out.second);
    edges.push_back({lhs_type, input_type});
    edges.push_back({rhs_type, input_type});
    return output_type;
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<CallNode> node) {
    for (auto arg : node->args) {
        arg->accept(*this);
    }
    auto ret_type = func_map[node->proc_name]->ret_type;
    auto tmp_type = std::make_shared<LType>(LPrim::Generic);
    edges.push_back({tmp_type, ret_type});
    return tmp_type;
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<FnNode> node) {
    node->stmts->accept(*this);
    return std::make_shared<LType>(LPrim::Invalid);
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<IfNode> node) {
    auto cond_type = node->condition->accept(*this);
    edges.push_back({cond_type, std::make_shared<LType>(LPrim::Bool)});
    node->thens->accept(*this);
    if (node->elses) {
        node->elses->accept(*this);
    }
    return std::make_shared<LType>(LPrim::Invalid);
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<LiteralNode> node) {
    std::shared_ptr<LType> literal_type = nullptr;
    switch (node->literal_type) {
        case LiteralType::Int:
            literal_type = std::make_shared<LType>(LPrim::Int);
            break;
        case LiteralType::Bool:
            literal_type = std::make_shared<LType>(LPrim::Bool);
            break;
    }
    assert(literal_type);
    return literal_type;
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<ProgramNode> node) {
    for (auto node : node->fns) {
        auto fn = std::dynamic_pointer_cast<FnNode>(node);
        if (fn) {
            func_map[fn->name] = fn;
        }
        else {
            std::cerr << "Program error" << std::endl;
            exit(1);
        }
    }

    for (auto node : node->fns) {
        auto fn = std::static_pointer_cast<FnNode>(node);
        curr_func = fn->name;
        node->accept(*this);
    }

    return std::make_shared<LType>(LPrim::Invalid);
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<RetNode> node) {
    auto expr_type = node->expr->accept(*this); 
    auto ret_type = func_map.at(curr_func)->ret_type;

    edges.push_back({ret_type, expr_type});
    
    return std::make_shared<LType>(LPrim::Invalid);
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    for (auto stmt : node->stmts) {
        stmt->accept(*this);
    }
    return std::make_shared<LType>(LPrim::Invalid);
}

std::pair<LPrim, LPrim> op_types(UnaryOp op) {
    switch (op) {
        case UnaryOp::NOT:
            return {LPrim::Bool, LPrim::Bool};
    }
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr_type = node->expr->accept(*this);

    auto in_out = op_types(node->op);
    auto input_type = std::make_shared<LType>(in_out.first);
    auto output_type = std::make_shared<LType>(in_out.second);
    edges.push_back({expr_type, input_type});
    return output_type;
}

std::shared_ptr<LType> TypeVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    return node->var.impl->type_info;
}


