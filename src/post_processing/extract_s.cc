
#include <iostream>
#include "extract_s.h"
#include <cassert>
#include "ast_node.h"
#include "extract_fns.h"

std::vector<std::shared_ptr<ASTNode>> extract_s(
    ParseNode root
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);
    std::vector<std::shared_ptr<ASTNode>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program

        ParseNode fns = root.children.at(1);
        result = extract_fns(fns);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}
