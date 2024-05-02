
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <cassert>

#include "eval_visitor.h"
#include "extract_s.h"
#include "lang_type.h"
#include "scope_vars_visitor.h"
#include "type_check.h"
#include "lang_scanning.h"
#include "lang_parsing.h"
#include "type_visitor.h"

void relax_edges(std::vector<std::pair<std::shared_ptr<LType>, std::shared_ptr<LType>>> edges) {
    int n = edges.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auto& [t1_gen, t2_gen] = edges[j];
            assert(std::holds_alternative<LPrim>(*t1_gen));
            assert(std::holds_alternative<LPrim>(*t2_gen));

            auto& t1 = std::get<LPrim>(*t1_gen);
            auto& t2 = std::get<LPrim>(*t2_gen);

            if (t2 == LPrim::Int || t2 == LPrim::Bool) {
                if (t1 == t2 || t1 == LPrim::Generic) {
                    t1 = t2;
                }
                else {
                    std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
                    exit(1);
                }
            }
        }
    }

}

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

        auto type_graph = tv.edges;
        relax_edges(type_graph);

        for (auto node : std::static_pointer_cast<ProgramNode>(program_node2)->fns) {
            auto fn = std::static_pointer_cast<FnNode>(node);
            std::cout << fn->name << ": ";
            for (auto param : fn->params) {
                std::cout << to_string(*param.impl->type_info) << " ";
            }
            std::cout << "-> " << to_string(*fn->ret_type) << std::endl;
        }

        EvalVisitor ev;
        program_node->accept(ev);
        for (auto& [name, func] : ev.funcs) {
            if (name == "main") {
                auto result = func({});
                std::cout << "Result: " << result << std::endl;
            }
        }
    }
}

