#include "parse_node.h"

std::vector<State> ParseNode::get_production() {
    std::vector<State> result = {state};
    for (auto& child : children) {
        result.push_back(child.state);
    }
    return result;
}

std::string ParseNode::to_string(int depth) {
    std::string indent = std::string(depth, ' ');
    std::string result;

    result = indent + state::to_string(state) + " " + lexeme + "\n";
    for (auto& child : children) {
        result += child.to_string(depth + 2);
    }

    return result;
}
