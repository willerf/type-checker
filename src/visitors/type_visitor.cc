
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
    auto lhs_type = node->lhs.impl->type_info;
    auto rhs_type = node->rhs->accept(*this);
    ltg.union_types(lhs_type, rhs_type);
    return make_lt(LPrim::Invalid);
}

std::map<BinaryOp, LTypeClass> bin_op_tc = {
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
    {BinaryOp::MOD, LTypeClass::Percent}
};
PtrLType TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs_type = node->lhs->accept(*this);
    auto rhs_type = node->rhs->accept(*this);
    auto result_type = ltg.union_types(lhs_type, rhs_type);

    if (bin_op_tc.contains(node->op)) {
        return ltg.add_tc(result_type, bin_op_tc.at(node->op));
    }
    else {
        auto bool_type = make_lt(LPrim::Bool);
        ltg.add_type(bool_type);
        return ltg.union_types(result_type, bool_type);
    }
}

PtrLType TypeVisitor::visit(std::shared_ptr<CallNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<FnNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<IfNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<LiteralNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<ProgramNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<RetNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
}

PtrLType TypeVisitor::visit(std::shared_ptr<VarAccessNode> node) {
}


