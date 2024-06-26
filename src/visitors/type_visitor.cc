
#include "type_visitor.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "array_access_node.h"
#include "binary_expr_node.h"
#include "fn_node.h"
#include "lang_type.h"
#include "unary_expr_node.h"
#include "unreachable_error.h"
#include "var_decl_node.h"
#include "visitor.h"

PtrLType TypeVisitor::visit(std::shared_ptr<ArrayAccessNode> node) {
    auto access_target = node->access_target->accept(*this);
    auto index = node->index->accept(*this);

    auto int_type = make_lt(LPrim::Int);
    ltg.add_type(int_type);
    ltg.union_types(index, int_type);

    if (!std::holds_alternative<LArray>(**access_target)) {
        auto elem_type = make_lt(LGeneric {});
        ltg.add_type(elem_type);
        auto arr_type = make_lt(LArray {elem_type});
        ltg.add_type(arr_type);
        ltg.union_types(access_target, arr_type);
    }

    return std::get<LArray>(**access_target).ltype;
}

PtrLType TypeVisitor::visit(std::shared_ptr<ArrayNode> node) {
    auto array_elem_type = make_lt(LGeneric {});
    ltg.add_type(array_elem_type);
    for (auto elem : node->init_list) {
        auto elem_type = elem->accept(*this);
        ltg.union_types(array_elem_type, elem_type);
    }

    if (node->init_size) {
        auto size_type = node->init_size->accept(*this);

        auto int_type = make_lt(LPrim::Int);
        ltg.add_type(int_type);
        ltg.union_types(size_type, int_type);
    }

    auto array_type = make_lt(LArray {array_elem_type});
    return ltg.add_type(array_type);
}

PtrLType TypeVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto lhs_type = node->lhs->accept(*this);
    auto rhs_type = node->rhs->accept(*this);
    ltg.union_types(lhs_type, rhs_type);
    return make_lt(LPrim::Invalid);
}

static std::map<BinaryOp, LTypeClass> bin_op_tc = {
    {BinaryOp::EQ, LTypeClass::Eq},
    {BinaryOp::NE, LTypeClass::Eq},
    {BinaryOp::LT, LTypeClass::Ord},
    {BinaryOp::GT, LTypeClass::Ord},
    {BinaryOp::LE, LTypeClass::Ord},
    {BinaryOp::GE, LTypeClass::Ord},
    {BinaryOp::PLUS, LTypeClass::Plus},
    {BinaryOp::MINUS, LTypeClass::Minus},
    {BinaryOp::TIMES, LTypeClass::Star},
    {BinaryOp::DIVIDE, LTypeClass::Slash},
    {BinaryOp::MOD, LTypeClass::Percent}};
static std::set<BinaryOp> comp_ops = {
    BinaryOp::EQ,
    BinaryOp::NE,
    BinaryOp::LT,
    BinaryOp::GT,
    BinaryOp::LE,
    BinaryOp::GE};

PtrLType TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs_type = node->lhs->accept(*this);
    auto rhs_type = node->rhs->accept(*this);
    ltg.union_types(lhs_type, rhs_type);

    if (bin_op_tc.contains(node->op)) {
        auto result_type = ltg.add_tc(lhs_type, bin_op_tc.at(node->op));
        if (comp_ops.contains(node->op)) {
            auto bool_type = make_lt(LPrim::Bool);
            return ltg.add_type(bool_type);
        } else {
            return result_type;
        }
    } else {
        auto bool_type = make_lt(LPrim::Bool);
        ltg.add_type(bool_type);
        return ltg.union_types(lhs_type, bool_type);
    }
}

PtrLType TypeVisitor::visit(std::shared_ptr<CallNode> node) {
    std::vector<PtrLType> arg_types;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this));
    }
    return ltg.add_call(node->func_name, arg_types);
}

PtrLType TypeVisitor::visit(std::shared_ptr<FnNode> node) {
    for (auto param : node->params) {
        ltg.add_type(param.impl->ptr_ltype);
    }
    ltg.add_type(node->ret_type);
    node->stmts->accept(*this);
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<IfNode> node) {
    auto cond_type = node->condition->accept(*this);
    node->thens->accept(*this);
    if (node->elses) {
        node->elses->accept(*this);
    }
    auto bool_type = make_lt(LPrim::Bool);
    ltg.add_type(bool_type);
    return ltg.union_types(cond_type, bool_type);
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
        case LiteralType::Char:
            literal_type = make_lt(LPrim::Char);
            break;
        case LiteralType::Str:
            literal_type = make_lt(LPrim::Str);
            break;
    }
    assert(literal_type);
    ltg.add_type(literal_type);
    return literal_type;
}

PtrLType TypeVisitor::visit(std::shared_ptr<ProgramNode> node) {
    for (auto node : node->fns) {
        auto fn = std::static_pointer_cast<FnNode>(node);
        if (fn_map.contains(fn->name)) {
            std::cerr << "ERROR: Duplicate function names found: " << fn->name
                      << std::endl;
        }
        fn_map[fn->name] = fn;
        ltg.add_fn(fn->name, fn);
    }

    for (auto node : node->fns) {
        auto fn = std::static_pointer_cast<FnNode>(node);
        curr_fn = fn;
        node->accept(*this);
    }

    ltg.reduce();

    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<RetNode> node) {
    auto expr_type = node->expr->accept(*this);
    auto ret_type = curr_fn->ret_type;
    return ltg.union_types(expr_type, ret_type);
}

PtrLType TypeVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    for (auto stmt : node->stmts) {
        stmt->accept(*this);
    }
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr_type = node->expr->accept(*this);

    switch (node->op) {
        case UnaryOp::NOT:
            auto bool_type = make_lt(LPrim::Bool);
            ltg.add_type(bool_type);
            return ltg.union_types(expr_type, bool_type);
    }

    std::cerr << "UnaryExpr error" << std::endl;
    exit(1);
}

PtrLType TypeVisitor::visit(std::shared_ptr<VarDeclNode> node) {
    auto var_type = node->var.impl->ptr_ltype;
    ltg.add_type(var_type);
    auto rhs_type = node->rhs->accept(*this);
    ltg.union_types(var_type, rhs_type);
    return make_lt(LPrim::Invalid);
}

PtrLType TypeVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    return node->var.impl->ptr_ltype;
}

PtrLType TypeVisitor::visit(std::shared_ptr<WhileNode> node) {
    auto cond_type = node->condition->accept(*this);
    node->body->accept(*this);
    auto bool_type = make_lt(LPrim::Bool);
    ltg.add_type(bool_type);
    return ltg.union_types(cond_type, bool_type);
}
