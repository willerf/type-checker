
#include "eval_visitor_utils.h"

#include "lang_type_utils.h"
#include "unreachable_error.h"

LDataValue operator||(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const bool& lhs, const bool& rhs) {
                return LDataValue {lhs || rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator&&(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const bool& lhs, const bool& rhs) {
                return LDataValue {lhs && rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator==(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs == rhs};
            },
            [](const bool& lhs, const bool& rhs) {
                return LDataValue {lhs == rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs == rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs == rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator!=(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs != rhs};
            },
            [](const bool& lhs, const bool& rhs) {
                return LDataValue {lhs != rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs != rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs != rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator<(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs < rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs < rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs < rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator>(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs > rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs > rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs > rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator<=(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs <= rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs <= rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs <= rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator>=(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs >= rhs};
            },
            [](const char& lhs, const char& rhs) {
                return LDataValue {lhs >= rhs};
            },
            [](const std::string& lhs, const std::string& rhs) {
                return LDataValue {lhs >= rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator+(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs + rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator-(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs - rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator*(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs * rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator/(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs / rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator%(const LDataValue& lhs, const LDataValue& rhs) {
    return std::visit(
        overloaded {
            [](const int& lhs, const int& rhs) {
                return LDataValue {lhs % rhs};
            },
            [](const auto&, const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        lhs,
        rhs
    );
}

LDataValue operator!(const LDataValue& expr) {
    return std::visit(
        overloaded {
            [](const int& expr) { return LDataValue {!expr}; },
            [](const auto&) {
                UNREACHABLE;
                return LDataValue {std::monostate {}};
            }},
        expr
    );
}

std::string to_string(const LDataValue& expr) {
    return std::visit(
        overloaded {
            [](const std::monostate& expr) { return std::string("unit"); },
            [](const int& expr) { return std::to_string(expr); },
            [](const bool& expr) {
                return std::string(expr ? "true" : "false");
            },
            [](const char& expr) {
                return std::string(1, expr);
                ;
            },
            [](const std::string& expr) { return expr; },
            [](const LRetValue& expr) {
                UNREACHABLE;
                return std::string();
            },
        },
        expr
    );
}
