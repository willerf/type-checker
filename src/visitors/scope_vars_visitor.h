#pragma once

#include <functional>
#include <map>
#include <stack>

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

class ScopedVarsVisitor: public Visitor<std::shared_ptr<ASTNode>> {
    std::stack<std::map<std::string, Variable>> scopes;

  public:
    std::shared_ptr<ASTNode> visit(std::shared_ptr<ArrayNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<AssignNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<BinaryExprNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<CallNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<FnNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<IfNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<LiteralNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<ProgramNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<RetNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<StmtBlockNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<UnaryExprNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<VarAccessNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<VarDeclNode>) override;
    std::shared_ptr<ASTNode> visit(std::shared_ptr<WhileNode>) override;
};

class VariableNotFoundError {
  public:
    const std::string name;
    const size_t line_no;

    VariableNotFoundError(const std::string& name, size_t line_no);
};
