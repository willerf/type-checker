
#pragma once

#include <functional>
#include <map>
#include <variant>

#include "variable.h"

struct LArrayValue;

typedef std::variant<std::monostate, int, bool, char, std::string, LArrayValue>
    LRetValue;
typedef std::variant<
    std::monostate,
    int,
    bool,
    char,
    std::string,
    LArrayValue,
    LRetValue>
    LDataValue;

struct LArrayValue {
    std::vector<std::shared_ptr<LDataValue>> data;
};

typedef std::map<std::shared_ptr<VarImpl>, std::shared_ptr<LDataValue>>
    LEnvironment;
typedef std::variant<LDataValue, std::shared_ptr<LDataValue>> EvalFuncRet;
typedef std::function<EvalFuncRet(LEnvironment&)> EvalFunc;
typedef std::function<LDataValue(std::vector<LDataValue>)> CallableFunc;
