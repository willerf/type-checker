
#pragma once

#include "program_node.h"

void add_std_functions_to_ast(std::shared_ptr<ProgramNode> prog_node);
void add_std_function_impls(
    std::map<std::string, std::shared_ptr<CallableFunc>>& func_map
);
