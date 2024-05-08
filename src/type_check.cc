
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
#include "type_visitor2.h"


void relax_sub_edges(std::vector<std::pair<std::shared_ptr<LType>, std::shared_ptr<LType>>>& edges) {
    int n = edges.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            auto& [t1_gen, t2_gen] = edges[j];
            
            bool t1_prim = std::holds_alternative<LPrim>(*t1_gen);
            bool t2_prim = std::holds_alternative<LPrim>(*t2_gen);

            if (t1_prim && t2_prim) {
                auto& t1 = std::get<LPrim>(*t1_gen);
                auto& t2 = std::get<LPrim>(*t2_gen);
                if (t1 == LPrim::Generic && t2 == LPrim::Generic || t1 == t2) {
                    continue;
                }
                else if (t1 == LPrim::Generic) {
                    t1 = t2; 
                }
                else if (t2 == LPrim::Generic) {
                    continue;
                }
                else {
                    std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
                    exit(1);
                }
            }
            else if (t1_prim) {
                auto& t1 = std::get<LPrim>(*t1_gen);
                auto& t2 = std::get<std::vector<LTypeClass>>(*t2_gen);

                if (t1 == LPrim::Generic) {
                    *t1_gen = *t2_gen;
                }
                else {
                    // todo: add checks for compability between prim type and type classes
                }
            }
            else if (t2_prim) {
                auto& t1 = std::get<std::vector<LTypeClass>>(*t1_gen);
                auto& t2 = std::get<LPrim>(*t2_gen);

                if (t2 == LPrim::Generic) {
                }
                else {
                    // todo: add checks for compability between prim type and type classes
                }
            }
            else {
                auto& t1 = std::get<std::vector<LTypeClass>>(*t1_gen);
                auto& t2 = std::get<std::vector<LTypeClass>>(*t2_gen);

                std::vector<LTypeClass> tcs;
                for (auto tc : t1) {
                    tcs.push_back(tc);
                }
                for (auto tc : t2) {
                    if (std::find(tcs.begin(), tcs.end(), tc) == tcs.end()) {
                        tcs.push_back(tc);
                    }
                }
                t1 = tcs;
            }
        }
    }
}



void relax_eq_edges(std::vector<std::pair<std::shared_ptr<LType>, std::shared_ptr<LType>>>& edges, std::map<std::shared_ptr<LType>, int>& edges_id) {
    int n = edges.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auto& [t1_gen, t2_gen] = edges[j];
            int edge_id = std::min(edges_id.at(t1_gen), edges_id.at(t2_gen));
            edges_id[t1_gen] = edge_id;
            edges_id[t2_gen] = edge_id;
            
            bool t1_prim = std::holds_alternative<LPrim>(*t1_gen);
            bool t2_prim = std::holds_alternative<LPrim>(*t2_gen);

            if (t1_prim && t2_prim) {
                auto& t1 = std::get<LPrim>(*t1_gen);
                auto& t2 = std::get<LPrim>(*t2_gen);
                if (t1 == LPrim::Generic && t2 == LPrim::Generic || t1 == t2) {
                    continue;
                }
                else if (t1 == LPrim::Generic) {
                    t1 = t2; 
                }
                else if (t2 == LPrim::Generic) {
                    t2 = t1;
                }
                else {
                    std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
                    exit(1);
                }
            }
            else if (t1_prim) {
                auto& t1 = std::get<LPrim>(*t1_gen);
                auto& t2 = std::get<std::vector<LTypeClass>>(*t2_gen);

                if (t1 == LPrim::Generic) {
                    *t1_gen = *t2_gen;
                }
                else {
                    // todo: add checks for compability between prim type and type classes
                    *t2_gen = *t1_gen;
                }
            }
            else if (t2_prim) {
                auto& t1 = std::get<std::vector<LTypeClass>>(*t1_gen);
                auto& t2 = std::get<LPrim>(*t2_gen);

                if (t2 == LPrim::Generic) {
                    *t2_gen = *t1_gen;
                }
                else {
                    // todo: add checks for compability between prim type and type classes
                    *t1_gen = *t2_gen;
                }
            }
            else {
                auto& t1 = std::get<std::vector<LTypeClass>>(*t1_gen);
                auto& t2 = std::get<std::vector<LTypeClass>>(*t2_gen);

                std::vector<LTypeClass> tcs;
                for (auto tc : t1) {
                    tcs.push_back(tc);
                }
                for (auto tc : t2) {
                    if (std::find(tcs.begin(), tcs.end(), tc) == tcs.end()) {
                        tcs.push_back(tc);
                    }
                }
                t1 = tcs;
                t2 = tcs;
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

        auto eq_edges = tv.eq_edges;
        auto sub_edges = tv.sub_edges;

        int i = 0;
        std::map<std::shared_ptr<LType>, int> edges_id;
        for (auto [start, end] : eq_edges) {
            edges_id[start] = i;
            i++;
            edges_id[end] = i;
            i++;
        }

        relax_eq_edges(eq_edges, edges_id);
        relax_sub_edges(sub_edges);
        relax_eq_edges(eq_edges, edges_id);

        for (auto node : std::static_pointer_cast<ProgramNode>(program_node2)->fns) {
            std::string output = "";
            auto fn = std::static_pointer_cast<FnNode>(node);

            char c = 'a';
            std::map<int, char> type_vars;
            output += fn->name + " [";
            for (auto param : fn->params) {
                auto type_info = param.impl->type_info;
                if (std::holds_alternative<LPrim>(*type_info) && std::get<LPrim>(*type_info) == LPrim::Generic) {
                    if (!type_vars.contains(edges_id[type_info])) {
                        type_vars[edges_id[type_info]] = c;
                        output += "\'";
                        output += c;
                        output += ", ";
                        c++;
                    }
                }
                else if (std::holds_alternative<std::vector<LTypeClass>>(*type_info)) {
                    auto& tcs = std::get<std::vector<LTypeClass>>(*type_info);
                    if (!type_vars.contains(edges_id[type_info])) {
                        type_vars[edges_id[type_info]] = c;
                        output += "\'";
                        output += c;
                        output += " ";
                        output += to_string(tcs);
                        output += ", ";
                    }
                }
            }
           
            if (!(std::holds_alternative<LPrim>(*fn->ret_type) && std::get<LPrim>(*fn->ret_type) != LPrim::Generic) 
                    && type_vars.contains(edges_id[fn->ret_type])) {
                std::cerr << "unreachable" << std::endl;
            }
            if (!type_vars.empty()) {
                output.pop_back();
                output.pop_back();
            }
            output += "]: (";

            for (auto param : fn->params) {
                auto type_info = param.impl->type_info;
                if (std::holds_alternative<LPrim>(*type_info) && std::get<LPrim>(*type_info) != LPrim::Generic) {
                    output += to_string(*type_info);
                    output += ", ";
                }
                else { 
                    output += "\'";
                    output += type_vars[edges_id[type_info]];
                    output += ", ";
                }
            }
            if (!fn->params.empty()) {
                output.pop_back();
                output.pop_back();
            }
            
            output += ") -> ";
            auto type_info = fn->ret_type;
            if (std::holds_alternative<LPrim>(*type_info) && std::get<LPrim>(*type_info) != LPrim::Generic) {
                output += to_string(*type_info);
            }
            else { 
                output += "\'";
                output += type_vars[edges_id[type_info]];
            }
            std::cout << output << std::endl;
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

