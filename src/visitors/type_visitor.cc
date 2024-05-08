
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
}

PtrLType TypeVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
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


