
#pragma once

#include <functional>
#include <map>
#include <vector>

#include "lang_type.h"
#include "visitor.h"

struct ASTNode {
    size_t line_no = SIZE_T_MAX;
    virtual void accept(Visitor<void>& visitor) = 0;
    virtual std::shared_ptr<ASTNode>
    accept(Visitor<std::shared_ptr<ASTNode>>& visitor) = 0;
    virtual std::function<int(std::map<std::string, int>&)>
    accept(Visitor<std::function<int(std::map<std::string, int>&)>>& visitor
    ) = 0;
    virtual LType
    accept(Visitor<LType>& visitor) = 0;
    virtual std::shared_ptr<LType>
    accept(Visitor<std::shared_ptr<LType>>& visitor) = 0;
    virtual std::shared_ptr<ASTNode> clone() const = 0;

    virtual ~ASTNode() {}
};
