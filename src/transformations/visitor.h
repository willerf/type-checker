
#pragma once

#include <memory>

struct ASTNode;
struct StmtBlockNode;
struct FnNode;

template<typename T>
class Visitor {
  public:
    virtual T visit(std::shared_ptr<ASTNode>);
    virtual T visit(std::shared_ptr<StmtBlockNode>);
    virtual T visit(std::shared_ptr<FnNode>);

    virtual ~Visitor() {}
};
