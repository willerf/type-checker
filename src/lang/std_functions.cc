

#include "std_functions.h"

#include <iostream>

#include "eval_visitor_utils.h"
#include "fn_node.h"
#include "program_node.h"
#include "stmt_block_node.h"

void add_std_functions_to_ast(std::shared_ptr<ProgramNode> prog_node) {
    prog_node->fns.push_back(
        make_fn("print", {Variable {}}, make_stmt_block({}), LGeneric({}))
    );
    prog_node->fns.push_back(
        make_fn("println", {Variable {}}, make_stmt_block({}), LGeneric({}))
    );
}

void add_std_function_impls(
    std::map<std::string, std::shared_ptr<CallableFunc>>& func_map
) {
    (*func_map["print"]) = [=](auto args) {
        std::cout << to_string(args.at(0));
        return LDataValue {std::monostate {}};
    };
    (*func_map["println"]) = [=](auto args) {
        std::cout << to_string(args.at(0)) << std::endl;
        return LDataValue {std::monostate {}};
    };
}
