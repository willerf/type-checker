
#include "extract_fns.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "parse_node.h"
#include "state.h"
// #include "visit_params.h"

void extract_fns(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ParseNode fn = root.children.at(0);
        extract_fn(fn);
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ParseNode fn = root.children.at(0);
        extract_fn(fn);

        // extract rest of functions
        ParseNode fns = root.children.at(1);
        extract_fns(fns);
    } else {
        std::cerr << "Invalid production found while extracting fns."
                  << std::endl;
        exit(1);
    }
}

void extract_fn(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {

        // extract function name
        ParseNode id = root.children.at(1);
        std::string name = id.lexeme;

        // extract function parameters
        ParseNode optparams = root.children.at(3);

    } else {
        std::cerr << "Invalid production found while extracting fn."
                  << std::endl;
        exit(1);
    }
}


