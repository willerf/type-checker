
#include "extract_vardef.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "lang_type.h"
#include "parse_node.h"
#include "state.h"
#include "variable.h"

Variable extract_vardef(ParseNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::vardef);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::vardef, Terminal::ID}) {
        // extract variable definition
        ParseNode id = root.children.at(0);
        std::string name = id.lexeme;

        return Variable(name);
    } else {
        std::cerr << "Invalid production found while processing vardef."
                  << std::endl;
        exit(1);
    }
}
