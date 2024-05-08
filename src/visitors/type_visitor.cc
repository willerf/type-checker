
#include "type_visitor.h"

#include <cassert>
#include <iostream>
#include <memory>
#include "lang_type.h"
#include "unary_expr_node.h"



PtrLType TypeVisitor::visit(std::shared_ptr<ASTNode> node) {
    std::cerr << "TypeVisitor error" << std::endl;
    exit(1);
}

PtrLType TypeVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto expr_type = node->rhs->accept(*this);
    eq_edges.push_back({expr_type, node->lhs.impl->type_info});
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs_type = node->lhs->accept(*this);
    auto rhs_type = node->rhs->accept(*this);
    eq_edges.push_back({lhs_type, rhs_type});

    PtrLType out_type = nullptr;
    switch (node->op) {
        case BinaryOp::OR:
        case BinaryOp::AND:
            eq_edges.push_back({lhs_type, make_lt(LPrim::Bool)}); 
            out_type = make_lt(LPrim::Bool);
            break;
        case BinaryOp::EQ:
        case BinaryOp::NE:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Eq)});
            out_type = make_lt(LPrim::Bool);
            break;
        case BinaryOp::LT:
        case BinaryOp::GT:
        case BinaryOp::LE:
        case BinaryOp::GE:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Ord)});
            out_type = make_lt(LPrim::Bool);
            break;
        case BinaryOp::PLUS:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Plus)});
            out_type = make_lt(LPrim::Generic);
            eq_edges.push_back({lhs_type, out_type});
            break;
        case BinaryOp::MINUS:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Minus)});
            out_type = make_lt(LPrim::Generic);
            eq_edges.push_back({lhs_type, out_type});
        case BinaryOp::TIMES:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Star)});
            out_type = make_lt(LPrim::Generic);
            eq_edges.push_back({lhs_type, out_type});
        case BinaryOp::DIVIDE:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Slash)});
            out_type = make_lt(LPrim::Generic);
            eq_edges.push_back({lhs_type, out_type});
        case BinaryOp::MOD:
            eq_edges.push_back({lhs_type, make_lt(LTypeClass::Percent)});
            out_type = make_lt(LPrim::Generic);
            eq_edges.push_back({lhs_type, out_type});
            break;
    }
    assert(out_type);
    return out_type;
}

PtrLType TypeVisitor::visit(std::shared_ptr<CallNode> node) {
    if (!func_map.contains(node->proc_name)) {
        std::cerr << "Invalid function call on line " << node->line_no << std::endl;
        exit(1);
    }
    auto fn = func_map[node->proc_name];
    if (fn->params.size() != node->args.size()) {
        std::cerr << "Invalid function call on line " << node->line_no << std::endl;
        exit(1);
    }
    for (int i = 0; i < fn->params.size(); i++) {
        auto param_type = fn->params[i].impl->type_info;
        auto arg = node->args[i];

        auto arg_type = arg->accept(*this);
        sub_edges.push_back({param_type, arg_type});
    }
    auto ret_type = fn->ret_type;
    auto tmp_type = make_lt(LPrim::Generic);
    sub_edges.push_back({tmp_type, ret_type});
    return tmp_type;
}

PtrLType TypeVisitor::visit(std::shared_ptr<FnNode> node) {
    node->stmts->accept(*this);
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<IfNode> node) {
    auto cond_type = node->condition->accept(*this);
    eq_edges.push_back({cond_type, make_lt(LPrim::Bool)});
    node->thens->accept(*this);
    if (node->elses) {
        node->elses->accept(*this);
    }
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<LiteralNode> node) {
    PtrLType literal_type = nullptr;
    switch (node->literal_type) {
        case LiteralType::Int:
            literal_type = make_lt(LPrim::Int);
            break;
        case LiteralType::Bool:
            literal_type = make_lt(LPrim::Bool);
            break;
    }
    assert(literal_type);
    return literal_type;
}

PtrLType TypeVisitor::visit(std::shared_ptr<ProgramNode> node) {
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

    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<RetNode> node) {
    auto expr_type = node->expr->accept(*this); 
    auto ret_type = func_map.at(curr_func)->ret_type;

    eq_edges.push_back({ret_type, expr_type});
    
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    for (auto stmt : node->stmts) {
        stmt->accept(*this);
    }
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr_type = node->expr->accept(*this);
        
    PtrLType out_type = nullptr;
    switch (node->op) {
        case UnaryOp::NOT:
            eq_edges.push_back({expr_type, make_lt(LPrim::Bool)}); 
            out_type = make_lt(LPrim::Bool);
            break;
    }

    assert(out_type);
    return out_type;
}

PtrLType TypeVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    return node->var.impl->type_info;
}


