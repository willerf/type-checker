
#pragma once

#include <functional>
#include <map>
#include <vector>

#include "eval_visitor_types.h"
#include "lang_type.h"
#include "visitor.h"

struct ASTNode {
    size_t line_no = SIZE_T_MAX;
    virtual void accept(Visitor<void>& visitor) = 0;
    virtual std::shared_ptr<ASTNode>
    accept(Visitor<std::shared_ptr<ASTNode>>& visitor) = 0;
    virtual EvalFunc accept(Visitor<EvalFunc>& visitor) = 0;
    virtual PtrLType accept(Visitor<PtrLType>& visitor) = 0;
    virtual std::shared_ptr<ASTNode> clone() const = 0;

    virtual ~ASTNode() {}
};
