
#pragma once

#include <functional>
#include <map>
#include <variant>

#include "variable.h"

typedef std::variant<std::monostate, int, bool, char, std::string> LRetValue;
typedef std::variant<std::monostate, int, bool, char, std::string, LRetValue>
    LDataValue;
typedef std::map<std::shared_ptr<VarImpl>, LDataValue> LEnvironment;
typedef std::function<LDataValue(LEnvironment&)> EvalFunc;
typedef std::function<LDataValue(std::vector<LDataValue>)> CallableFunc;
