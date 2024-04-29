
#include "type_visitor.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "literal_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"

#include <iostream>
#include <cassert>

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<ASTNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<AssignNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<CallNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<FnNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<IfNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<LiteralNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<RetNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
}

std::shared_ptr<ASTNode> EvalVisitor::visit(std::shared_ptr<VarAccessNode> node) {
}

