
#include "program_node.h"

#include <memory>
#include <vector>

ProgramNode::ProgramNode(std::vector<std::shared_ptr<ASTNode>> fns) : fns{fns} {}

std::shared_ptr<ProgramNode> make_program(std::vector<std::shared_ptr<ASTNode>> fns) {
    return std::make_shared<ProgramNode>(fns);
}

