
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <cassert>

#include "extract_s.h"
#include "lang_type.h"
#include "scope_vars_visitor.h"
#include "type_check.h"
#include "lang_scanning.h"
#include "lang_parsing.h"
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

        TypeVisitor tv;
        program_node2->accept(tv);

    }
}

