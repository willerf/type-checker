
#pragma once

#include <vector>

#include "visitor.h"

struct ASTNode {
    virtual void accept(Visitor<void>& visitor) = 0;
    virtual std::shared_ptr<ASTNode> accept(Visitor<std::shared_ptr<ASTNode>>& visitor
    ) = 0;
    virtual std::shared_ptr<ASTNode> clone() const = 0;

    virtual ~ASTNode() {}
};
