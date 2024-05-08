
#pragma once

#include <functional>
#include <map>
#include <queue>
#include <set>

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
#include "visitor.h"

class TypeVisitor: public Visitor<PtrLType> {
    std::map<std::string, std::shared_ptr<FnNode>> func_map;
    std::string curr_func;
    
  public:
    std::vector<std::pair<PtrLType, PtrLType>> eq_edges;
    std::vector<std::pair<PtrLType, PtrLType>> sub_edges;

    PtrLType visit(std::shared_ptr<ASTNode>) override;
    PtrLType visit(std::shared_ptr<AssignNode>) override;
    PtrLType
        visit(std::shared_ptr<BinaryExprNode>) override;
    PtrLType visit(std::shared_ptr<CallNode>) override;
    PtrLType visit(std::shared_ptr<FnNode>) override;
    PtrLType visit(std::shared_ptr<IfNode>) override;
    PtrLType visit(std::shared_ptr<LiteralNode>) override;
    PtrLType visit(std::shared_ptr<ProgramNode>) override;
    PtrLType visit(std::shared_ptr<RetNode>) override;
    PtrLType visit(std::shared_ptr<StmtBlockNode>) override;
    PtrLType visit(std::shared_ptr<UnaryExprNode>) override;
    PtrLType visit(std::shared_ptr<VarAccessNode>) override;
};

