
#include "extract_expr.h"

#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>

#include "array_node.h"
#include "binary_expr_node.h"
#include "call_node.h"
#include "extract_args.h"
#include "lang_type.h"
#include "literal_node.h"
#include "state.h"
#include "unary_expr_node.h"
#include "var_access_node.h"
#include "variable.h"

static std::set<NonTerminal> expr_non_terminals = {
    NonTerminal::expr,
    NonTerminal::p1,
    NonTerminal::p2,
    NonTerminal::p3,
    NonTerminal::p4,
    NonTerminal::p5,
    NonTerminal::p6,
    NonTerminal::p7,
    NonTerminal::p8};

std::shared_ptr<ASTNode> extract_expr(ParseNode root) {
    assert(
        std::holds_alternative<NonTerminal>(root.state)
        && expr_non_terminals.count(std::get<NonTerminal>(root.state))
    );
    std::shared_ptr<ASTNode> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::p8, Terminal::ID}) {
        ParseNode id = root.children.at(0);
        std::string name = id.lexeme;
        result = make_var_access(Variable(name));
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::NUM}) {
        ParseNode num = root.children.at(0);
        result = make_literal(LiteralType::Int, num.lexeme);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::TRUE}) {
        ParseNode expr = root.children.at(0);
        result = make_literal(LiteralType::Bool, expr.lexeme);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::FALSE}) {
        ParseNode expr = root.children.at(0);
        result = make_literal(LiteralType::Bool, expr.lexeme);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::CHARLITERAL}) {
        ParseNode expr = root.children.at(0);
        if (expr.lexeme.length() == 3) {
            result = make_literal(LiteralType::Char, expr.lexeme.substr(1, 1));
        } else {
            throw std::runtime_error(
                "Character literal not one character on line "
                + std::to_string(expr.line_no)
            );
        }
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::STRLITERAL}) {
        ParseNode expr = root.children.at(0);
        result = make_literal(
            LiteralType::Str,
            expr.lexeme.substr(1, expr.lexeme.length() - 2)
        );
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::LBRACKET, NonTerminal::optargs, Terminal::RBRACKET}) {
        ParseNode optargs = root.children.at(1);
        auto args = extract_optargs(optargs);

        result = make_array(args, nullptr);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::LBRACKET, NonTerminal::optargs, Terminal::RBRACKET, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}) {
        ParseNode optargs = root.children.at(1);
        auto args = extract_optargs(optargs);

        ParseNode expr_node = root.children.at(4);
        auto expr = extract_expr(expr_node);

        result = make_array(args, expr);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::STRLITERAL}) {
        ParseNode expr = root.children.at(0);
        result = make_literal(
            LiteralType::Str,
            expr.lexeme.substr(1, expr.lexeme.length() - 2)
        );
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}) {
        ParseNode expr = root.children.at(1);
        result = extract_expr(expr);
    } else if (prod == std::vector<State> {NonTerminal::p8, Terminal::ID, Terminal::LPAREN, NonTerminal::optargs, Terminal::RPAREN}) {
        ParseNode id = root.children.at(0);
        std::string name = id.lexeme;

        ParseNode optargs = root.children.at(2);
        auto args = extract_optargs(optargs);

        result = make_call(name, args);
    } else if (root.children.size() == 1 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state))) {
        // recursively call into next operator precedence layer
        ParseNode expr = root.children.at(0);
        result = extract_expr(expr);
    }
    else if (root.children.size() == 2 && std::holds_alternative<Terminal>(root.children.at(0).state) && std::holds_alternative<NonTerminal>(root.children.at(1).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(1).state))) {
        // extract unary operator
        ParseNode lhs_op = root.children.at(0);
        ParseNode expr = root.children.at(1);

        Terminal unary_op = std::get<Terminal>(lhs_op.state);
        auto expr_node = extract_expr(expr);
        switch (unary_op) {
            case Terminal::NOT:
                result = make_unary_expr(UnaryOp::NOT, expr_node);
                break;
            default:
                std::cerr
                    << "Unimplemented unary operator found while processing expr."
                    << std::endl;
                exit(1);
        }
    }
    else if (root.children.size() == 3 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state)) && std::holds_alternative<Terminal>(root.children.at(1).state) && std::holds_alternative<NonTerminal>(root.children.at(2).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(2).state))) {
        // extract binary operator
        ParseNode lhs = root.children.at(0);
        ParseNode mid = root.children.at(1);
        ParseNode rhs = root.children.at(2);

        auto lhs_node = extract_expr(lhs);
        Terminal mid_op = std::get<Terminal>(mid.state);
        auto rhs_node = extract_expr(rhs);

        switch (mid_op) {
            case Terminal::OR:
                result = make_binary_expr(lhs_node, BinaryOp::OR, rhs_node);
                break;
            case Terminal::AND:
                result = make_binary_expr(lhs_node, BinaryOp::AND, rhs_node);
                break;
            case Terminal::PLUS:
                result = make_binary_expr(lhs_node, BinaryOp::PLUS, rhs_node);
                break;
            case Terminal::MINUS:
                result = make_binary_expr(lhs_node, BinaryOp::MINUS, rhs_node);
                break;
            case Terminal::STAR:
                result = make_binary_expr(lhs_node, BinaryOp::TIMES, rhs_node);
                break;
            case Terminal::SLASH:
                result = make_binary_expr(lhs_node, BinaryOp::DIVIDE, rhs_node);
                break;
            case Terminal::PCT:
                result = make_binary_expr(lhs_node, BinaryOp::MOD, rhs_node);
                break;
            case Terminal::EQ:
                result = make_binary_expr(lhs_node, BinaryOp::EQ, rhs_node);
                break;
            case Terminal::NE:
                result = make_binary_expr(lhs_node, BinaryOp::NE, rhs_node);
                break;
            case Terminal::LT:
                result = make_binary_expr(lhs_node, BinaryOp::LT, rhs_node);
                break;
            case Terminal::GT:
                result = make_binary_expr(lhs_node, BinaryOp::GT, rhs_node);
                break;
            case Terminal::LE:
                result = make_binary_expr(lhs_node, BinaryOp::LE, rhs_node);
                break;
            case Terminal::GE:
                result = make_binary_expr(lhs_node, BinaryOp::GE, rhs_node);
                break;
            default:
                std::cerr
                    << "Unimplemented binary operator found while processing expr."
                    << std::endl;
                exit(1);
        }
    } else {
        std::cerr << "Invalid production found while processing expr."
                  << std::endl;
        exit(1);
    }

    assert(result);
    size_t line_no = SIZE_T_MAX;
    for (auto& child : root.children) {
        line_no = std::min(line_no, child.line_no);
    }
    result->line_no = std::min(result->line_no, line_no);
    return result;
}
