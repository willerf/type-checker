
#include "execute.h"

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
#include "type_printer.h"
#include "type_visitor.h"

void execute(std::vector<std::string> input_file_paths) {
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

        std::static_pointer_cast<ProgramNode>(program_node2)
            ->fns.push_back(make_fn(
                "print",
                {Variable {}},
                make_stmt_block({}),
                LGeneric({})
            ));
        std::static_pointer_cast<ProgramNode>(program_node2)
            ->fns.push_back(make_fn(
                "println",
                {Variable {}},
                make_stmt_block({}),
                LGeneric({})
            ));

        TypeVisitor tv;
        program_node2->accept(tv);

        std::static_pointer_cast<ProgramNode>(program_node2);

        EvalVisitor ev;
        program_node2->accept(ev);

        std::shared_ptr<CallableFunc> prog_main = nullptr;
        for (auto& [name, func] : ev.func_map) {
            if (name == "main") {
                prog_main = func;
            }
        }
        if (!prog_main) {
            std::cerr << "ERROR: Missing main function" << std::endl;
            exit(1);
        }

        auto result = (*prog_main)({});
    }
}
