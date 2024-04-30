
#include "extract_args.h"
#include <cassert>
#include <memory>
#include <iostream>
#include "ast_node.h"
#include "extract_expr.h"

std::vector<std::shared_ptr<ASTNode>> extract_args(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::args);
    std::vector<std::shared_ptr<ASTNode>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr}) {
        // extract singular argument
        ParseNode expr = root.children.at(0);
        auto expr_node = extract_expr(expr);
        expr_node->line_no = root.line_no;
        result.push_back(
                expr_node
        );
    } else if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr, Terminal::COMMA, NonTerminal::args}) {
        // extract code argument
        ParseNode expr = root.children.at(0);
        auto expr_node = extract_expr(expr);
        expr_node->line_no = root.line_no;
        result.push_back(
                expr_node
        );

        // extract rest of arguments
        ParseNode args = root.children.at(2);
        auto child_result =
            extract_args(args);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing args."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<ASTNode>> extract_optargs(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optargs);
    std::vector<std::shared_ptr<ASTNode>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optargs}) {
        // function call passing in no arguments
    } else if (prod == std::vector<State> {NonTerminal::optargs, NonTerminal::args}) {
        // extract arguments
        ParseNode args = root.children.at(0);
        result = extract_args(args);
    } else {
        std::cerr << "Invalid production found while processing optargs."
                  << std::endl;
        exit(1);
    }

    return result;
}
