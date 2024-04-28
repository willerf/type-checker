#pragma once

#include <memory>

#include "ast_node.h"

template<typename T>
struct ASTClone: ASTNode {
    std::shared_ptr<ASTNode> clone() const override {
        return std::make_shared<T>(static_cast<const T&>(*this));
    }
};
