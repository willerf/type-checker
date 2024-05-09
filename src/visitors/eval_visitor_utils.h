#pragma once

#include <string>

#include "eval_visitor_types.h"

LDataValue operator||(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator&&(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator==(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator!=(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator<(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator>(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator<=(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator>=(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator+(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator-(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator*(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator/(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator%(const LDataValue& lhs, const LDataValue& rhs);
LDataValue operator!(const LDataValue& expr);

std::string to_string(const LDataValue& expr);
