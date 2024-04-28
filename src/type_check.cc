
#include <fstream>
#include <iostream>
#include <sstream>

#include "type_check.h"

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
        try {
            auto tokens = scan(input);
            auto ast_node = parse(tokens);
            auto result_list = extract_symbols(ast_node, program_context);
            import_list.insert(
                import_list.end(),
                result_list.begin(),
                result_list.end()
            );
            modules.push_back({input_file_path, ast_node});
        } catch (CompileError& compile_error) {
            compile_error.input_file_path = input_file_path;
            throw;
        }
    }

}

