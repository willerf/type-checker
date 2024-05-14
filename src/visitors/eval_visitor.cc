
#include "eval_visitor.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>

#include "array_access_node.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "eval_visitor_types.h"
#include "eval_visitor_utils.h"
#include "fn_node.h"
#include "if_node.h"
#include "lang_type_utils.h"
#include "literal_node.h"
#include "program_node.h"
#include "ret_node.h"
#include "std_functions.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "unreachable_error.h"
#include "var_access_node.h"
#include "var_decl_node.h"
#include "visitor.h"
#include "while_node.h"

static LDataValue
get_val(std::variant<LDataValue, std::shared_ptr<LDataValue>> val) {
    return std::visit(
        overloaded {
            [](LDataValue& v) { return v; },
            [](std::shared_ptr<LDataValue>& v) { return *v; },
        },
        val
    );
}

EvalFunc EvalVisitor::visit(std::shared_ptr<ArrayAccessNode> node) {
    auto access_target_expr = node->access_target->accept(*this);
    auto index_expr = node->index->accept(*this);
    return [=](auto& env) {
        auto access_target = get_val(access_target_expr(env));
        auto index = get_val(index_expr(env));
        return std::get<LArrayValue>(access_target)
            .data.at(std::get<int>(index));
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<ArrayNode> node) {
    std::vector<EvalFunc> init_list;
    for (auto init_val : node->init_list) {
        init_list.push_back(init_val->accept(*this));
    }

    if (node->init_size) {
        auto init_size = node->init_size->accept(*this);
        return [=](auto& env) {
            size_t size = std::get<int>(get_val(init_size(env)));
            std::vector<std::shared_ptr<LDataValue>> arr(size);
            for (int i = 0; i < size; i++) {
                arr[i] = std::make_shared<LDataValue>(std::monostate {});
            }
            for (int i = 0; i < std::min(size, init_list.size()); i++) {
                arr[i] =
                    std::make_shared<LDataValue>(get_val(init_list[i](env)));
            }
            return LArrayValue {arr};
        };
    } else {
        return [=](auto& env) {
            auto size = init_list.size();
            std::vector<std::shared_ptr<LDataValue>> arr(size);
            for (int i = 0; i < size; i++) {
                arr[i] =
                    std::make_shared<LDataValue>(get_val(init_list[i](env)));
            }
            return LArrayValue {arr};
        };
    }
}

EvalFunc EvalVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto lhs_expr = node->lhs->accept(*this);
    auto rhs_expr = node->rhs->accept(*this);
    return [=](auto& env) {
        auto lhs = lhs_expr(env);
        auto rhs = rhs_expr(env);
        if (std::holds_alternative<LDataValue>(lhs)) {
            std::cerr << "ERROR: Cannot assign to expression found on line "
                      << node->line_no << std::endl;
        }
        *std::get<std::shared_ptr<LDataValue>>(lhs) = get_val(rhs);
        return LDataValue {std::monostate {}};
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    switch (node->op) {
        case BinaryOp::OR:
            return [=](auto& env) {
                return get_val(lhs(env)) || get_val(rhs(env));
            };
        case BinaryOp::AND:
            return [=](auto& env) {
                return get_val(lhs(env)) && get_val(rhs(env));
            };
        case BinaryOp::EQ:
            return [=](auto& env) {
                return get_val(lhs(env)) == get_val(rhs(env));
            };
        case BinaryOp::NE:
            return [=](auto& env) {
                return get_val(lhs(env)) != get_val(rhs(env));
            };
        case BinaryOp::LT:
            return [=](auto& env) {
                return get_val(lhs(env)) < get_val(rhs(env));
            };
        case BinaryOp::GT:
            return [=](auto& env) {
                return get_val(lhs(env)) > get_val(rhs(env));
            };
        case BinaryOp::LE:
            return [=](auto& env) {
                return get_val(lhs(env)) <= get_val(rhs(env));
            };
        case BinaryOp::GE:
            return [=](auto& env) {
                return get_val(lhs(env)) >= get_val(rhs(env));
            };
        case BinaryOp::PLUS:
            return [=](auto& env) {
                return get_val(lhs(env)) + get_val(rhs(env));
            };
        case BinaryOp::MINUS:
            return [=](auto& env) {
                return get_val(lhs(env)) - get_val(rhs(env));
            };
        case BinaryOp::TIMES:
            return [=](auto& env) {
                return get_val(lhs(env)) * get_val(rhs(env));
            };
        case BinaryOp::DIVIDE:
            return [=](auto& env) {
                return get_val(lhs(env)) / get_val(rhs(env));
            };
        case BinaryOp::MOD:
            return [=](auto& env) {
                return get_val(lhs(env)) % get_val(rhs(env));
            };
    }
}

EvalFunc EvalVisitor::visit(std::shared_ptr<CallNode> node) {
    auto func = func_map.at(node->func_name);
    std::vector<EvalFunc> args;
    for (auto arg : node->args) {
        args.push_back(arg->accept(*this));
    }
    return [=](auto& env) {
        std::vector<LDataValue> arg_vals;
        for (auto& arg : args) {
            arg_vals.push_back(get_val(arg(env)));
        }
        return (*func)(arg_vals);
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<FnNode> node) {
    auto stmtblock = node->stmts->accept(*this);
    std::vector<std::shared_ptr<VarImpl>> params;
    for (auto param : node->params) {
        params.push_back(param.impl);
    }
    auto func = [=](auto args) {
        assert(args.size() == params.size());
        LEnvironment env;
        for (int i = 0; i < params.size(); i++) {
            env[params.at(i)] = std::make_shared<LDataValue>(args.at(i));
        }
        auto result = get_val(stmtblock(env));
        if (std::holds_alternative<LRetValue>(result)) {
            return std::visit(
                overloaded {
                    [&](const std::monostate& val) { return LDataValue {val}; },
                    [&](const int& val) { return LDataValue {val}; },
                    [&](const bool& val) { return LDataValue {val}; },
                    [&](const char& val) { return LDataValue {val}; },
                    [&](const std::string& val) { return LDataValue {val}; },
                    [&](const LArrayValue& val) { return LDataValue {val}; },
                },
                std::get<LRetValue>(result)
            );
        } else {
            return LDataValue {std::monostate {}};
        }
    };
    (*func_map[node->name]) = func;
    return [](auto& env) {
        UNREACHABLE;
        return LDataValue {std::monostate {}};
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<IfNode> node) {
    auto condition = node->condition->accept(*this);
    auto thens = node->thens->accept(*this);
    EvalFunc elses = nullptr;
    if (node->elses) {
        elses = node->elses->accept(*this);
    }
    if (elses) {
        return [=](auto& env) {
            auto result = get_val(condition(env));
            if (std::get<bool>(result)) {
                return thens(env);
            } else {
                return elses(env);
            }
        };
    } else {
        return [=](auto& env) {
            auto result = get_val(condition(env));
            if (std::get<bool>(result)) {
                return thens(env);
            }
            return EvalFuncRet {LDataValue {std::monostate {}}};
        };
    }
}

EvalFunc EvalVisitor::visit(std::shared_ptr<LiteralNode> node) {
    LDataValue val;
    switch (node->literal_type) {
        case LiteralType::Int:
            val = stoi(node->value);
            break;
        case LiteralType::Bool:
            val = node->value == "true";
            break;
        case LiteralType::Char:
            val = node->value.at(0);
            break;
        case LiteralType::Str:
            val = node->value;
            break;
    }
    return [=](auto& env) { return val; };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<ProgramNode> node) {
    for (auto fn_node : node->fns) {
        auto fn = std::static_pointer_cast<FnNode>(fn_node);
        func_map[fn->name] = std::make_shared<CallableFunc>(nullptr);
    }
    for (auto fn_node : node->fns) {
        fn_node->accept(*this);
    }
    add_std_function_impls(func_map);
    return [](auto& env) {
        UNREACHABLE;
        return LDataValue {std::monostate {}};
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<RetNode> node) {
    auto expr = node->expr->accept(*this);
    return [=](auto& env) {
        auto val = get_val(expr(env));
        return std::visit(
            overloaded {
                [&](const std::monostate& val) {
                    return LDataValue {LRetValue {val}};
                },
                [&](const int& val) { return LDataValue {LRetValue {val}}; },
                [&](const bool& val) { return LDataValue {LRetValue {val}}; },
                [&](const char& val) { return LDataValue {LRetValue {val}}; },
                [&](const std::string& val) {
                    return LDataValue {LRetValue {val}};
                },
                [&](const LArrayValue& val) {
                    return LDataValue {LRetValue {val}};
                },
                [&](const LRetValue& val) {
                    UNREACHABLE;
                    return LDataValue {std::monostate {}};
                },
            },
            val
        );
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    std::vector<EvalFunc> stmts;
    for (auto stmt : node->stmts) {
        stmts.push_back(stmt->accept(*this));
    }
    return [=](auto& env) {
        auto env_copy = env;
        for (auto stmt : stmts) {
            auto val = get_val(stmt(env));
            if (std::holds_alternative<LRetValue>(val)) {
                LEnvironment result;
                std::set_intersection(
                    env.begin(),
                    env.end(),
                    env_copy.begin(),
                    env_copy.end(),
                    std::inserter(result, result.begin()),
                    [](const auto& a, const auto& b) {
                        return a.first < b.first;
                    }
                );
                env.clear();
                env = std::move(result);
                return val;
            }
        }
        LEnvironment result;
        std::set_intersection(
            env.begin(),
            env.end(),
            env_copy.begin(),
            env_copy.end(),
            std::inserter(result, result.begin()),
            [](const auto& a, const auto& b) { return a.first < b.first; }
        );
        env.clear();
        env = std::move(result);
        return LDataValue {std::monostate {}};
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr = node->expr->accept(*this);
    switch (node->op) {
        case UnaryOp::NOT:
            return [=](auto& env) { return !get_val(expr(env)); };
    }
}

EvalFunc EvalVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    auto var = node->var.impl;
    return [=](auto& env) { return env[var]; };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<VarDeclNode> node) {
    auto var = node->var.impl;
    auto rhs = node->rhs->accept(*this);
    return [=](auto& env) {
        env[var] = std::make_shared<LDataValue>(get_val(rhs(env)));
        return LDataValue {std::monostate {}};
    };
}

EvalFunc EvalVisitor::visit(std::shared_ptr<WhileNode> node) {
    auto condition = node->condition->accept(*this);
    auto body = node->body->accept(*this);
    return [=](auto& env) {
        auto result = get_val(condition(env));
        while (std::get<bool>(result)) {
            auto val = get_val(body(env));
            if (!std::holds_alternative<std::monostate>(val)) {
                return val;
            }
            result = get_val(condition(env));
        }
        return LDataValue {std::monostate {}};
    };
}
