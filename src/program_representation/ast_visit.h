
#pragma once

#include <functional>
#include <map>
#include <vector>

#include "ast_clone.h"

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

    std::function<int(std::map<std::string, int>&)>
    accept(Visitor<std::function<int(std::map<std::string, int>&)>>& visitor
    ) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }

    LType
    accept(Visitor<LType>& visitor) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }

    std::shared_ptr<LType>
    accept(Visitor<std::shared_ptr<LType>>& visitor) override {
        return visitor.visit(std::static_pointer_cast<T>(this->shared_from_this(
        )));
    }
};
