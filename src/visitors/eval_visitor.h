#pragma once

#include <functional>
#include <map>

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
#include "visitor.h"

class EvalVisitor:
    public Visitor<std::function<int(std::map<std::string, int>&)>> {
  public:
    std::map<std::string, std::function<int(std::vector<int>)>> funcs;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<ASTNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<AssignNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<BinaryExprNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<CallNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<FnNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<IfNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<LiteralNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<RetNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<StmtBlockNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<UnaryExprNode>) override;
    std::function<int(std::map<std::string, int>&)>
        visit(std::shared_ptr<VarAccessNode>) override;
};
