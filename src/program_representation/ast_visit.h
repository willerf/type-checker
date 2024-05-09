
#pragma once

#include <functional>
#include <map>
#include <vector>

#include "ast_clone.h"
#include "eval_visitor_types.h"

template<typename T>
struct ASTVisit: ASTClone<T>, std::enable_shared_from_this<ASTVisit<T>> {
    void accept(Visitor<void>& visitor) override {
        visitor.visit(std::static_pointer_cast<T>(this->shared_from_this()));
    }

    std::shared_ptr<ASTNode> accept(Visitor<std::shared_ptr<ASTNode>>& visitor
    ) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }

    EvalFunc accept(Visitor<EvalFunc>& visitor) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }

    PtrLType accept(Visitor<PtrLType>& visitor) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }
};
