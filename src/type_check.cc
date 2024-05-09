
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <cassert>
#include <variant>

#include "extract_s.h"
#include "lang_type.h"
#include "scope_vars_visitor.h"
#include "type_check.h"
#include "lang_scanning.h"
#include "lang_parsing.h"
#include "type_visitor.h"
#include "lang_type_utils.h"

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

        for (auto node : std::static_pointer_cast<ProgramNode>(program_node2)->fns) {
            std::string output = "";
            auto fn = std::static_pointer_cast<FnNode>(node);
            
            std::map<LType, char> type_vars;
            char c = 'a';
            for (auto param : fn->params) {
                auto ltype = *param.impl->ptr_ltype;
                if (!type_vars.contains(ltype)) {
                    type_vars[ltype] = c++;
                }
            }

            output += fn->name + " [";
            for (auto& [ltype, c] : type_vars) {
                std::visit(overloaded{
                    [](LPrim& lprim) {},
                    [&](LGeneric& tcs) {
                        output += "\'";
                        output += type_vars.at(ltype);
                        for (auto& tc : tcs) {
                            output += " " + to_string(tc);
                        }
                        output += ", ";
                    },
                    [](LCustom& lcustom) {}
                }, *ltype);
            }

            if (!output.ends_with("[")) {
                output.pop_back();
                output.pop_back();
            }

            output += "]: (";
            for (auto param : fn->params) {
                auto ltype = *param.impl->ptr_ltype;
                std::visit(overloaded{
                    [&](LPrim& lprim) {
                        output += to_string(ltype) + ", ";
                    },
                    [&](LGeneric& tcs) {
                        output += "\'";
                        output += type_vars.at(ltype);
                        output += ", ";
                    },
                    [&](LCustom& lcustom) {
                        output += to_string(ltype) + ", ";
                    }
                }, *ltype);
            }
            if (!fn->params.empty()) {
                output.pop_back();
                output.pop_back();
            }
            output += ") -> ";

            
            auto ltype = *fn->ret_type;

            std::visit(overloaded{
                [&](LPrim& lprim) {
                    output += to_string(ltype);
                },
                [&](LGeneric& tcs) {
                    output += "\'";
                    output += type_vars.at(ltype);
                },
                [&](LCustom& lcustom) {
                    output += to_string(ltype);
                }
            }, *ltype);


            std::cout << output << std::endl; 
        }

    }
}

