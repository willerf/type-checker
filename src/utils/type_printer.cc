#include "type_printer.h"

#include <iostream>

#include "fn_node.h"
#include "lang_type_utils.h"
#include "program_node.h"

void print_types(std::shared_ptr<ProgramNode> program_node) {
    for (auto node : program_node->fns) {
        std::string output = "";
        auto fn = std::static_pointer_cast<FnNode>(node);

        char c = 'a';
        std::map<LType, char> type_vars;
        std::function<void(LType)> add_single_type = [&](LType ltype) {
            std::visit(
                overloaded {
                    [](LPrim& lprim) {},
                    [&](LGeneric& tcs) {
                        if (!type_vars.contains(ltype)) {
                            type_vars[ltype] = c;
                            output += "\'";
                            output += type_vars.at(ltype);
                            for (auto& tc : tcs) {
                                output += " " + to_string(tc);
                            }
                            output += ", ";
                            c++;
                        }
                    },
                    [&](LArray& larray) { add_single_type(*larray.ltype); }},
                *ltype
            );
        };

        output += fn->name + " {";
        for (auto param : fn->params) {
            auto ltype = *param.impl->ptr_ltype;
            add_single_type(ltype);
        }

        if (!output.ends_with("{")) {
            output.pop_back();
            output.pop_back();
            output += "}";
        } else {
            output.pop_back();
            output.pop_back();
        }

        std::function<std::string(LType)> to_string_single_type =
            [&](LType ltype) {
                return std::visit(
                    overloaded {
                        [&](LPrim& lprim) { return to_string(ltype); },
                        [&](LGeneric& tcs) {
                            return std::string("\'")
                                + std::string(1, type_vars.at(ltype));
                        },
                        [&](LArray& larray) {
                            return "[" + to_string_single_type(*larray.ltype)
                                + "]";
                        }},
                    *ltype
                );
            };

        output += ": (";
        for (auto param : fn->params) {
            auto ltype = *param.impl->ptr_ltype;
            output += to_string_single_type(ltype) + ", ";
        }
        if (!fn->params.empty()) {
            output.pop_back();
            output.pop_back();
        }
        output += ") -> ";

        auto ltype = *fn->ret_type;
        output += to_string_single_type(ltype);

        std::cout << output << std::endl;
    }
}
