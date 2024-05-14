
#pragma once

#include <functional>
#include <map>
#include <queue>
#include <set>

#include "array_node.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "lang_type_graph.h"
#include "literal_node.h"
#include "program_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"
#include "var_decl_node.h"
#include "visitor.h"
#include "while_node.h"

class TypeVisitor: public Visitor<PtrLType> {
    std::shared_ptr<FnNode> curr_fn;
    std::map<std::string, std::shared_ptr<FnNode>> fn_map;

  public:
    LangTypeGraph ltg;
    PtrLType visit(std::shared_ptr<ArrayAccessNode>) override;
    PtrLType visit(std::shared_ptr<ArrayNode>) override;
    PtrLType visit(std::shared_ptr<AssignNode>) override;
    PtrLType visit(std::shared_ptr<BinaryExprNode>) override;
    PtrLType visit(std::shared_ptr<CallNode>) override;
    PtrLType visit(std::shared_ptr<FnNode>) override;
    PtrLType visit(std::shared_ptr<IfNode>) override;
    PtrLType visit(std::shared_ptr<LiteralNode>) override;
    PtrLType visit(std::shared_ptr<ProgramNode>) override;
    PtrLType visit(std::shared_ptr<RetNode>) override;
    PtrLType visit(std::shared_ptr<StmtBlockNode>) override;
    PtrLType visit(std::shared_ptr<UnaryExprNode>) override;
    PtrLType visit(std::shared_ptr<VarAccessNode>) override;
    PtrLType visit(std::shared_ptr<VarDeclNode>) override;
    PtrLType visit(std::shared_ptr<WhileNode>) override;
};
