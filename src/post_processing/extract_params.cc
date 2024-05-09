
#include "extract_params.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <variant>

#include "extract_vardef.h"
#include "state.h"

struct ProgramContext;

std::vector<Variable> extract_params(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::params);
    std::vector<Variable> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef}) {
        // extract singular parameter
        ParseNode vardef = root.children.at(0);
        result.push_back(extract_vardef(vardef));
    } else if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}) {
        // extract code parameter
        ParseNode vardef = root.children.at(0);
        result.push_back(extract_vardef(vardef));

        // extract rest of parameters
        ParseNode params = root.children.at(2);
        auto child_result = extract_params(params);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing params."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<Variable> extract_optparams(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optparams);
    std::vector<Variable> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optparams}) {
        // function has no parameters
    } else if (prod == std::vector<State> {NonTerminal::optparams, NonTerminal::params}) {
        // extract parameters
        ParseNode params = root.children.at(0);
        result = extract_params(params);
    } else {
        std::cerr << "Invalid production found while processing optparams."
                  << std::endl;
        exit(1);
    }

    return result;
}
