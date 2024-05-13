
#include "type_check.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

#include "eval_visitor.h"
#include "eval_visitor_types.h"
#include "eval_visitor_utils.h"
#include "extract_s.h"
#include "lang_parsing.h"
#include "lang_scanning.h"
#include "lang_type.h"
#include "lang_type_utils.h"
#include "scope_vars_visitor.h"
#include "std_functions.h"
#include "type_printer.h"
#include "type_visitor.h"

void type_check(std::vector<std::string> input_file_paths) {
    for (std::string input_file_path : input_file_paths) {
        std::ifstream file {input_file_path};
        if (!file) {
            std::cerr << "Invalid file path: " << input_file_path << std::endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();
        auto tokens = scan(input);
        auto parse_node = parse(tokens);
        std::shared_ptr<ASTNode> program_node = extract_s(parse_node);

        ScopedVarsVisitor svv;
        auto program_node2 = program_node->accept(svv);

        add_std_functions_to_ast(
            std::static_pointer_cast<ProgramNode>(program_node2)
        );

        TypeVisitor tv;
        program_node2->accept(tv);

        print_types(std::static_pointer_cast<ProgramNode>(program_node2));
    }
}
