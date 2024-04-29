
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "eval_visitor.h"
#include "extract_s.h"
#include "type_check.h"
#include "lang_scanning.h"
#include "lang_parsing.h"

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
        std::vector<std::shared_ptr<ASTNode>> nodes = extract_s(parse_node);

        EvalVisitor ev;
        for (auto& node : nodes) {
            node->accept(ev);
        }
        for (auto& [name, func] : ev.funcs) {
            if (name == "main") {
                std::cout << "Result: " << func({}) << std::endl;
            }
        }
    }

}

