
#include "extract_fns.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "extract_stmts.h"
#include "fn_node.h"
#include "parse_node.h"
#include "state.h"
#include "extract_params.h"

std::vector<std::shared_ptr<ASTNode>> extract_fns(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);
    std::vector<std::shared_ptr<ASTNode>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ParseNode fn = root.children.at(0);
        result.push_back(extract_fn(fn));
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ParseNode fn = root.children.at(0);
        result.push_back(extract_fn(fn));

        // extract rest of functions
        ParseNode fns = root.children.at(1);
        auto rest_of_fns = extract_fns(fns);
        result.insert(result.end(), rest_of_fns.begin(), rest_of_fns.end());
    } else {
        std::cerr << "Invalid production found while extracting fns."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<ASTNode> extract_fn(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);
    std::shared_ptr<ASTNode> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {

        // extract function name
        ParseNode id = root.children.at(1);
        std::string name = id.lexeme;

        // extract function parameters
        ParseNode optparams = root.children.at(3);
        auto params = extract_optparams(optparams);

        ParseNode stmtblock = root.children.at(5);
        auto stmts = extract_stmtblock(stmtblock);

        result = make_fn(name, params, stmts);
    } else {
        std::cerr << "Invalid production found while extracting fn."
                  << std::endl;
        exit(1);
    }

    assert(result);
    result->line_no = root.children.at(1).line_no;
    return result;
}


