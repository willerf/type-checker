
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
    virtual T visit(std::shared_ptr<ASTNode>);
    virtual T visit(std::shared_ptr<AssignNode>);
    virtual T visit(std::shared_ptr<BinaryExprNode>);
    virtual T visit(std::shared_ptr<CallNode>);
    virtual T visit(std::shared_ptr<FnNode>);
    virtual T visit(std::shared_ptr<IfNode>);
    virtual T visit(std::shared_ptr<LiteralNode>);
    virtual T visit(std::shared_ptr<RetNode>);
    virtual T visit(std::shared_ptr<StmtBlockNode>);
    virtual T visit(std::shared_ptr<UnaryExprNode>);
    virtual T visit(std::shared_ptr<VarAccessNode>);

    virtual ~Visitor() {}
};
