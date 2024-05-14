
#include "extract_stmts.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "assign_node.h"
#include "ast_node.h"
#include "extract_expr.h"
#include "extract_vardef.h"
#include "if_node.h"
#include "parse_node.h"
#include "ret_node.h"
#include "var_decl_node.h"
#include "while_node.h"

std::shared_ptr<ASTNode> extract_stmt(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmt);
    std::shared_ptr<ASTNode> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::stmt,
            Terminal::LET,
            NonTerminal::vardef,
            Terminal::ASSIGN,
            NonTerminal::expr,
            Terminal::SEMI}) {
        // extract variable declaration and assignment
        ParseNode vardef = root.children.at(1);
        auto var = extract_vardef(vardef);

        ParseNode expr_node = root.children.at(3);
        auto expr = extract_expr(expr_node);

        result = make_var_decl(var, expr);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::ASSIGN, NonTerminal::expr, Terminal::SEMI}) {
        // extract variable declaration and assignment
        ParseNode lhs_node = root.children.at(0);
        auto lhs = extract_expr(lhs_node);

        ParseNode rhs_node = root.children.at(2);
        auto rhs = extract_expr(rhs_node);

        result = make_assign(lhs, rhs);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock, Terminal::ELSE, NonTerminal::stmtblock}) {
        // extract if else statements
        ParseNode expr = root.children.at(2);
        std::shared_ptr<ASTNode> condition = extract_expr(expr);

        ParseNode stmtblock_thens = root.children.at(4);
        auto thens = extract_stmtblock(stmtblock_thens);

        ParseNode stmtblock_elses = root.children.at(6);
        auto elses = extract_stmtblock(stmtblock_elses);

        result = make_if(condition, thens, elses);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock}) {
        // extract if statements
        ParseNode expr = root.children.at(2);
        std::shared_ptr<ASTNode> condition = extract_expr(expr);

        ParseNode stmtblock_thens = root.children.at(4);
        auto thens = extract_stmtblock(stmtblock_thens);

        result = make_if(condition, thens, nullptr);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::WHILE, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock}) {
        // extract while statements
        ParseNode expr = root.children.at(2);
        std::shared_ptr<ASTNode> condition = extract_expr(expr);

        ParseNode stmtblock_body = root.children.at(4);
        auto body = extract_stmtblock(stmtblock_body);

        result = make_while(condition, body);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::RET, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ParseNode expr_node = root.children.at(1);
        std::shared_ptr<ASTNode> expr = extract_expr(expr_node);

        result = make_ret(expr);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::SEMI}) {
        ParseNode expr_node = root.children.at(0);
        std::shared_ptr<ASTNode> expr = extract_expr(expr_node);

        result = expr;
    } else {
        std::cerr << "Invalid production found while processing stmt."
                  << std::endl;
        exit(1);
    }

    assert(result);
    result->line_no = root.children.at(0).line_no;
    return result;
}

std::vector<std::shared_ptr<ASTNode>> extract_stmts(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmts);
    std::vector<std::shared_ptr<ASTNode>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::stmts}) {
    } else if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt, NonTerminal::stmts}) {
        // extract code statement
        ParseNode stmt = root.children.at(0);
        result.push_back(extract_stmt(stmt));

        // extract rest of statements
        ParseNode stmts = root.children.at(1);
        auto rest = extract_stmts(stmts);

        result.insert(result.end(), rest.begin(), rest.end());
    } else {
        std::cerr << "Invalid production found while processing stmts."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<ASTNode> extract_stmtblock(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmtblock);
    std::shared_ptr<ASTNode> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::stmtblock,
            Terminal::LBRACE,
            NonTerminal::stmts,
            Terminal::RBRACE}) {
        // extract statements

        ParseNode stmts_node = root.children.at(1);

        auto stmts = extract_stmts(stmts_node);

        result = make_stmt_block(stmts);
    } else {
        std::cerr << "Invalid production found while processing stmtblock."
                  << std::endl;
        exit(1);
    }

    assert(result);
    result->line_no = root.children.at(0).line_no;
    return result;
}
