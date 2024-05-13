
#pragma once

#include <memory>

struct ASTNode;
struct ArrayNode;
struct AssignNode;
struct BinaryExprNode;
struct CallNode;
struct FnNode;
struct IfNode;
struct WhileNode;
struct LiteralNode;
struct ProgramNode;
struct RetNode;
struct StmtBlockNode;
struct UnaryExprNode;
struct VarAccessNode;
struct VarDeclNode;

template<typename T>
class Visitor {
  public:
    virtual T visit(std::shared_ptr<ArrayNode>) = 0;
    virtual T visit(std::shared_ptr<AssignNode>) = 0;
    virtual T visit(std::shared_ptr<BinaryExprNode>) = 0;
    virtual T visit(std::shared_ptr<CallNode>) = 0;
    virtual T visit(std::shared_ptr<FnNode>) = 0;
    virtual T visit(std::shared_ptr<IfNode>) = 0;
    virtual T visit(std::shared_ptr<LiteralNode>) = 0;
    virtual T visit(std::shared_ptr<ProgramNode>) = 0;
    virtual T visit(std::shared_ptr<RetNode>) = 0;
    virtual T visit(std::shared_ptr<StmtBlockNode>) = 0;
    virtual T visit(std::shared_ptr<UnaryExprNode>) = 0;
    virtual T visit(std::shared_ptr<VarAccessNode>) = 0;
    virtual T visit(std::shared_ptr<VarDeclNode>) = 0;
    virtual T visit(std::shared_ptr<WhileNode>) = 0;

    virtual ~Visitor() {}
};
