#include "type_printer.h"

#include <iostream>

#include "fn_node.h"
#include "lang_type_utils.h"
#include "program_node.h"

void print_types(std::shared_ptr<ProgramNode> program_node) {
    for (auto node : program_node->fns) {
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
            std::visit(
                overloaded {
                    [](LPrim& lprim) {},
                    [&](LGeneric& tcs) {
                        output += "\'";
                        output += type_vars.at(ltype);
                        for (auto& tc : tcs) {
                            output += " " + to_string(tc);
                        }
                        output += ", ";
                    },
                    [](LArray& larray) {}},
                *ltype
            );
        }

        if (!output.ends_with("[")) {
            output.pop_back();
            output.pop_back();
        }

        output += "]: (";
        for (auto param : fn->params) {
            auto ltype = *param.impl->ptr_ltype;
            std::visit(
                overloaded {
                    [&](LPrim& lprim) { output += to_string(ltype) + ", "; },
                    [&](LGeneric& tcs) {
                        output += "\'";
                        output += type_vars.at(ltype);
                        output += ", ";
                    },
                    [&](LArray& larray) { output += to_string(ltype) + ", "; }},
                *ltype
            );
        }
        if (!fn->params.empty()) {
            output.pop_back();
            output.pop_back();
        }
        output += ") -> ";

        auto ltype = *fn->ret_type;

        std::visit(
            overloaded {
                [&](LPrim& lprim) { output += to_string(ltype); },
                [&](LGeneric& tcs) {
                    output += "\'";
                    output += type_vars.at(ltype);
                },
                [&](LArray& larray) { output += to_string(ltype); }},
            *ltype
        );

        std::cout << output << std::endl;
    }
}
