
#include "eval_visitor.h"
#include "assign_node.h"
#include "ast_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "fn_node.h"
#include "if_node.h"
#include "literal_node.h"
#include "program_node.h"
#include "ret_node.h"
#include "stmt_block_node.h"
#include "unary_expr_node.h"
#include "var_access_node.h"
#include "visitor.h"

#include <algorithm>
#include <iostream>
#include <cassert>
#include <iterator>

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<ASTNode> node) {
    std::cerr << "EvalVisitor error" << std::endl;
    return [](auto& m){ 
        return 0; 
    };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<AssignNode> node) {
    auto expr = node->rhs->accept(*this);
    auto name = node->lhs.impl->name;
    return [=](auto& m) {
        m[name] = expr(m);
        return INT_MIN;
    };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<BinaryExprNode> node) {
    auto lhs = node->lhs->accept(*this);
    auto rhs = node->rhs->accept(*this);
    switch (node->op) {
        case BinaryOp::OR:
            return [=](auto& m){ return lhs(m) || rhs(m); };
        case BinaryOp::AND:
            return [=](auto& m){ return lhs(m) && rhs(m); };
        case BinaryOp::EQ:
            return [=](auto& m){ return lhs(m) == rhs(m); };
        case BinaryOp::NE:
            return [=](auto& m){ return lhs(m) != rhs(m); };
        case BinaryOp::LT:
            return [=](auto& m){ return lhs(m) < rhs(m); };
        case BinaryOp::GT:
            return [=](auto& m){ return lhs(m) > rhs(m); };
        case BinaryOp::LE:
            return [=](auto& m){ return lhs(m) <= rhs(m); };
        case BinaryOp::GE:
            return [=](auto& m){ return lhs(m) >= rhs(m); };
        case BinaryOp::PLUS:
            return [=](auto& m){ return lhs(m) + rhs(m); };
        case BinaryOp::MINUS:
            return [=](auto& m){ return lhs(m) - rhs(m); };
        case BinaryOp::TIMES:
            return [=](auto& m){ return lhs(m) * rhs(m); };
        case BinaryOp::DIVIDE:
            return [=](auto& m){ return lhs(m) / rhs(m); };
        case BinaryOp::MOD:
            return [=](auto& m){ return lhs(m) % rhs(m); };
    }
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<CallNode> node) {
    auto func = funcs.at(node->func_name);
    std::vector<std::function<int(std::map<std::string, int>&)>> args;
    for (auto& arg : node->args) {
        args.push_back(arg->accept(*this));
    }
    return [=](auto& m) {
        std::vector<int> arg_vals;
        for (auto& arg : args) {
            arg_vals.push_back(arg(m));
        }
        return func(arg_vals);
    };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<FnNode> node) {
    auto func_ptr = std::make_shared<std::shared_ptr<std::function<int(std::vector<int>)>>>(nullptr);
    auto func_wrapper = [=](auto args){
        assert(*func_ptr);
        return (**func_ptr)(args);
    };
    funcs[node->name] = func_wrapper;
    auto stmtblock = node->stmts->accept(*this);
    std::vector<std::string> params;
    for (auto param : node->params) {
        params.push_back(param.impl->name);
    }
    auto func = [=](auto args){
        assert(args.size() == params.size());
        std::map<std::string, int> m;
        for (int i = 0; i < params.size(); i++) {
            m[params.at(i)] = args.at(i);
        }
        return stmtblock(m);
    };
    (*func_ptr) = std::make_shared<std::function<int(std::vector<int>)>>(func);
    return [](auto& m){ return 0; };
}

/*
std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<FnNode> node) {
    auto stmtblock = node->stmts->accept(*this);
    std::vector<std::string> params;
    for (auto param : node->params) {
        params.push_back(*param.name);
    }
    auto func = [=](auto args){
        assert(args.size() == params.size());
        std::map<std::string, int> m;
        for (int i = 0; i < params.size(); i++) {
            m[params.at(i)] = args.at(i);
        }
        return stmtblock(m);
    };
    funcs[node->name] = func;
    return [](auto& m){ return 0; };
}
*/

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<IfNode> node) {
    auto condition = node->condition->accept(*this);
    auto thens = node->thens->accept(*this);
    auto elses = node->elses->accept(*this);
    return [=](auto& m){
        if (condition(m)) {
            return thens(m);
        }
        else {
            return elses(m);
        }
    };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<LiteralNode> node) {
    int val;
    switch (node->literal_type) {
        case LiteralType::Int:
            val = stoi(node->value);
            break;
        case LiteralType::Bool:
            val = node->value == "true" ? 1 : 0;
            break;
    }
    return [=](auto& m){ return val; };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<ProgramNode> node) {
    funcs["print"] = [](auto args) {
        std::cout << args.at(0);
        return true;
    };
    funcs["println"] = [](auto args) {
        std::cout << args.at(0) << std::endl;
        return true;
    };
    for (auto fn : node->fns) {
        fn->accept(*this);
    } 
    return [](auto& m){ return 0; };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<RetNode> node) {
    return node->expr->accept(*this);
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<StmtBlockNode> node) {
    std::vector<std::function<int(std::map<std::string, int>&)>> stmts;
    for (auto stmt : node->stmts) {
        stmts.push_back(stmt->accept(*this));
    }
    return [=](auto& m) {
        std::map<std::string, int> m_copy = m;
        for (auto stmt : stmts) {
            int val = stmt(m);
            if (val != INT_MIN) {
                std::vector<std::pair<std::string, int>> result;
                std::set_intersection(m.begin(), m.end(), m_copy.begin(), m_copy.end(), std::back_inserter(result), [](const auto& a, const auto& b) { return a.first < b.first; });
                m.clear();
                for (const auto& p : result) {
                    m.insert(p);
                }
                return val;
            }
        }
        std::vector<std::pair<std::string, int>> result;
        std::set_intersection(m.begin(), m.end(), m_copy.begin(), m_copy.end(), std::back_inserter(result), [](const auto& a, const auto& b) { return a.first < b.first; });
        m.clear();
        for (const auto& p : result) {
            m.insert(p);
        }
        return INT_MIN;
    };
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<UnaryExprNode> node) {
    auto expr = node->expr->accept(*this);
    switch (node->op) {
        case UnaryOp::NOT:
            return [=](auto& m){ return !expr(m); };
    }
}

std::function<int(std::map<std::string, int>&)> EvalVisitor::visit(std::shared_ptr<VarAccessNode> node) {
    auto name = node->var.impl->name; 
    return [=](auto& m) {
        return m[name];
    };
}

