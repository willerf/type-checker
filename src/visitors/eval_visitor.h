#pragma once

#include <functional>
#include <map>

#include "array_node.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "eval_visitor_types.h"
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

class EvalVisitor: public Visitor<EvalFunc> {
  public:
    std::map<std::string, std::shared_ptr<CallableFunc>> func_map;
    EvalFunc visit(std::shared_ptr<ArrayNode>) override;
    EvalFunc visit(std::shared_ptr<AssignNode>) override;
    EvalFunc visit(std::shared_ptr<BinaryExprNode>) override;
    EvalFunc visit(std::shared_ptr<CallNode>) override;
    EvalFunc visit(std::shared_ptr<FnNode>) override;
    EvalFunc visit(std::shared_ptr<IfNode>) override;
    EvalFunc visit(std::shared_ptr<LiteralNode>) override;
    EvalFunc visit(std::shared_ptr<ProgramNode>) override;
    EvalFunc visit(std::shared_ptr<RetNode>) override;
    EvalFunc visit(std::shared_ptr<StmtBlockNode>) override;
    EvalFunc visit(std::shared_ptr<UnaryExprNode>) override;
    EvalFunc visit(std::shared_ptr<VarAccessNode>) override;
    EvalFunc visit(std::shared_ptr<VarDeclNode>) override;
    EvalFunc visit(std::shared_ptr<WhileNode>) override;
};
