
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
    virtual std::variant<LPrim, LCustom> accept(Visitor<std::variant<LPrim, LCustom>>& visitor) = 0;
    virtual std::shared_ptr<ASTNode> clone() const = 0;

    virtual ~ASTNode() {}
};
