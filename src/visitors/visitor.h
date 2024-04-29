
#pragma once

#include <memory>

struct ASTNode;
struct AssignNode;
struct BinaryExprNode;
struct CallNode;
struct FnNode;
struct IfNode;
struct LiteralNode;
struct RetNode;
struct StmtBlockNode;
struct UnaryExprNode;
struct VarAccessNode;

template<typename T>
class Visitor {
  public:
    virtual T visit(std::shared_ptr<ASTNode>) = 0;
    virtual T visit(std::shared_ptr<AssignNode>) = 0;
    virtual T visit(std::shared_ptr<BinaryExprNode>) = 0;
    virtual T visit(std::shared_ptr<CallNode>) = 0;
    virtual T visit(std::shared_ptr<FnNode>) = 0;
    virtual T visit(std::shared_ptr<IfNode>) = 0;
    virtual T visit(std::shared_ptr<LiteralNode>) = 0;
    virtual T visit(std::shared_ptr<RetNode>) = 0;
    virtual T visit(std::shared_ptr<StmtBlockNode>) = 0;
    virtual T visit(std::shared_ptr<UnaryExprNode>) = 0;
    virtual T visit(std::shared_ptr<VarAccessNode>) = 0;

    virtual ~Visitor() {}
};
