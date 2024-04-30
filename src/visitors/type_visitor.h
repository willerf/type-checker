
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

class TypeVisitor: public Visitor<std::variant<LPrim, LCustom>> {
    std::map<std::string, std::shared_ptr<FnNode>> m;
    std::map<
        std::pair<
            std::shared_ptr<FnNode>,
            std::vector<std::variant<LPrim, LCustom>>>,
        std::variant<LPrim, LCustom>>
        completed;

  public:
    std::variant<LPrim, LCustom> visit(std::shared_ptr<ASTNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<AssignNode>) override;
    std::variant<LPrim, LCustom>
        visit(std::shared_ptr<BinaryExprNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<CallNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<FnNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<IfNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<LiteralNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<ProgramNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<RetNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<StmtBlockNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<UnaryExprNode>) override;
    std::variant<LPrim, LCustom> visit(std::shared_ptr<VarAccessNode>) override;
};
