
#include "lang_type_graph.h"

#include <cassert>
#include <iostream>
#include "lang_type.h"
#include "lang_type_utils.h"

PtrLType LangTypeGraph::union_types(PtrLType t1, PtrLType t2) {
    std::cout << "DEBUG: {" << to_string(t1) << "} {" << to_string(t2) << "}" << std::endl;
    assert(type_id.contains(t1));
    assert(type_id.contains(t2));

    PtrLType result_type = nullptr;

    auto t1_gen = *t1;
    auto t2_gen = *t2;

    bool t1_prim = std::holds_alternative<LPrim>(*t1_gen);
    bool t2_prim = std::holds_alternative<LPrim>(*t2_gen);

    if (t1_prim && t2_prim) {
        auto& ltimpl1 = std::get<LPrim>(*t1_gen);
        auto& ltimpl2 = std::get<LPrim>(*t2_gen);
        if (ltimpl1 == ltimpl2) {
            result_type = t1;
        }
        else {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else if (t1_prim) {
        auto& ltimpl1 = std::get<LPrim>(*t1_gen);
        auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

        if (compatible(ltimpl1, ltimpl2)) {
            result_type = t1;
        }
        else {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else if (t2_prim) {
        auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
        auto& ltimpl2 = std::get<LPrim>(*t2_gen);
        
        if (compatible(ltimpl2, ltimpl1)) {
            result_type = t2;
        }
        else {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else {
        auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
        auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

        LGeneric tcs;
        tcs.insert(ltimpl1.begin(), ltimpl1.end());
        tcs.insert(ltimpl2.begin(), ltimpl2.end());
        result_type = make_lt(tcs); 
    }

    assert(result_type);

    size_t result_tid = count++;
    size_t tid1 = type_id.at(t1);
    size_t tid2 = type_id.at(t2);
    std::set<PtrLType> type_set = {result_type};
    for (auto ptr_ltype : type_sets.at(tid1)) {
        *ptr_ltype = *result_type;
        type_id[ptr_ltype] = result_tid;
        type_set.insert(ptr_ltype);
    }
    for (auto ptr_ltype : type_sets.at(tid2)) {
        *ptr_ltype = *result_type;
        type_id[ptr_ltype] = result_tid;
        type_set.insert(ptr_ltype);
    }
    /* 
    for (auto& [ret_type, fn_name, arg_types] : calls) {
        if (type_set.contains(ret_type)) {
            *ret_type = *result_type;
            type_id[ret_type] = result_tid;
        }
    }
    */
    
    type_sets[result_tid] = type_set;
    type_id[result_type] = result_tid;
    return result_type;
}

PtrLType LangTypeGraph::add_tc(PtrLType ptr_ltype, LTypeClass tc) {
    assert(type_id.contains(ptr_ltype));
   
    std::visit(overloaded{
        [&](LPrim& lprim) {
            std::cerr << "Type error: " << to_string(ptr_ltype) << " " << to_string(tc) << std::endl;
            exit(1);
        },
        [&](LGeneric& lgeneric) {
            lgeneric.insert(tc);
        },
        [&](LCustom& lcustom) {
            std::cerr << "Type error: " << to_string(ptr_ltype) << " " << to_string(tc) << std::endl;
            exit(1);
        }
    }, **ptr_ltype);

    return ptr_ltype;
}

PtrLType LangTypeGraph::add_type(PtrLType ptr_ltype) {
    assert(!type_id.contains(ptr_ltype));
    size_t result_tid = count++;
    type_id[ptr_ltype] = result_tid;
    type_sets[result_tid] = {ptr_ltype};
    assert(type_id.contains(ptr_ltype));
    return ptr_ltype;
}

PtrLType LangTypeGraph::add_call(std::string fn_name, std::vector<PtrLType> arg_types) {
    auto ptr_ltype = add_type(make_lt(LGeneric{}));
    calls.push_back({ptr_ltype, fn_name, arg_types});
    return ptr_ltype;
}

void LangTypeGraph::add_fn(std::string fn_name, std::shared_ptr<FnNode> fn) {
    fn_map[fn_name] = fn;
}


static void subtype(PtrLType t1, PtrLType t2) {

    auto t1_gen = *t1;
    auto t2_gen = *t2;

    bool t1_prim = std::holds_alternative<LPrim>(*t1_gen);
    bool t2_prim = std::holds_alternative<LPrim>(*t2_gen);

    if (t1_prim && t2_prim) {
        auto& ltimpl1 = std::get<LPrim>(*t1_gen);
        auto& ltimpl2 = std::get<LPrim>(*t2_gen);

        if (ltimpl1 != ltimpl2) {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else if (t1_prim) {
        auto& ltimpl1 = std::get<LPrim>(*t1_gen);
        auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

        if (!compatible(ltimpl1, ltimpl2)) {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else if (t2_prim) {
        auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
        auto& ltimpl2 = std::get<LPrim>(*t2_gen);

        if (compatible(ltimpl2, ltimpl1)) { 
            *t1_gen = *t2_gen;
        }
        else {
            std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
            exit(1);
        }
    }
    else {
        auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
        auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

        ltimpl1.insert(ltimpl2.begin(), ltimpl2.end());
    }
}

void LangTypeGraph::reduce() {

    int n = calls.size();
    for (int i = 0; i < n; i++) {
        auto& [ret_type, fn_name, arg_types] = calls[i]; 
        auto fn = fn_map.at(fn_name);
        assert(fn->params.size() == arg_types.size());

        for (int i = 0; i < fn->params.size(); i++) {
            if (type_id[fn->params[i].impl->ptr_ltype] == type_id[fn->ret_type]) {
                union_types(ret_type, arg_types[i]);
            }
        }
        
        for (int i = 0; i < fn->params.size(); i++) {
            auto p1 = fn->params[i].impl->ptr_ltype;
            for (int j = i + 1; j < fn->params.size(); j++) {
                auto p2 = fn->params[j].impl->ptr_ltype;
                if (type_id.at(p1) == type_id.at(p2)) {
                    union_types(arg_types[i], arg_types[j]);
                }
            }
        }
    }


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            auto& [ret_type, fn_name, arg_types] = calls[j]; 
            auto fn = fn_map.at(fn_name);

            subtype(ret_type, fn->ret_type);

            for (int i = 0; i < fn->params.size(); i++) {
                auto param_type = fn->params[i].impl->ptr_ltype;
                subtype(arg_types[i], param_type);
            }

            
            /*
            assert(fn->params.size() == arg_types.size());
            if (std::holds_alternative<LPrim>(**fn->ret_type)) {
                auto ltype = std::get<LPrim>(**fn->ret_type);
                if (ltype != LPrim::Generic) {
                    if (**ret_type != **fn->ret_type) {
                        std::cerr << "Incompatible types " << to_string(ret_type) << " and " << to_string(fn->ret_type) << std::endl;
                        exit(1);
                    }
                }
                **ret_type = ltype;
            }
            
            for (int i = 0; i < fn->params.size(); i++) {
                auto param_type = fn->params[i].impl->ptr_ltype;
                if (std::holds_alternative<LPrim>(**param_type)) {
                    auto ltype = std::get<LPrim>(**param_type);
                    **ret_type = ltype;
                }
                
            }
            */
            
            /*
            for (int i = 0; i < fn->params.size(); i++) {
                if (type_id[fn->params[i].impl->ptr_ltype] == type_id[fn->ret_type]) {
                    std::cout << "DEBUG AGAIN: " << to_string(arg_types[i]) << std::endl;
                    std::cout << type_id[ret_type] << " " << type_id[arg_types[i]] << std::endl;
                    //size_t tid = type_id.at(ret_type);
                    //for (auto ptr_ltype : type_sets.at(tid)) {
                    //    *ptr_ltype = *arg_types[i];
                    //}
                    **ret_type = **arg_types[i];
                }
            }
            */
        
            
            /*
            for (int i = 0; i < fn->params.size(); i++) {
                

                auto t1 = arg_types[i];
                auto t2 = fn->params[i].impl->ptr_ltype;

                auto t1_gen = *t1;
                auto t2_gen = *t2;



                bool t1_prim = std::holds_alternative<LPrim>(*t1_gen);
                bool t2_prim = std::holds_alternative<LPrim>(*t2_gen);

                if (t1_prim && t2_prim) {
                    auto& ltimpl1 = std::get<LPrim>(*t1_gen);
                    auto& ltimpl2 = std::get<LPrim>(*t2_gen);
                    if (ltimpl1 == LPrim::Generic && ltimpl2 == LPrim::Generic || ltimpl1 == ltimpl2) {
                    }
                    else if (ltimpl1 == LPrim::Generic) {
                        ltimpl1 = ltimpl2;
                    }
                    else if (ltimpl2 == LPrim::Generic) {
                    }
                    else {
                        std::cerr << "Type error: " << to_string(t1) << " " << to_string(t2) << std::endl;
                        exit(1);
                    }
                }
                else if (t1_prim) {
                    auto& ltimpl1 = std::get<LPrim>(*t1_gen);
                    auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

                    if (ltimpl1 == LPrim::Generic) {
                        *t1_gen = *t2_gen;
                    }
                    else {
                        // todo: add checks for compability between prim type and type classes
                    }
                }
                else if (t2_prim) {
                    auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
                    auto& ltimpl2 = std::get<LPrim>(*t2_gen);

                    if (ltimpl2 == LPrim::Generic) {
                    }
                    else {
                        // todo: add checks for compability between prim type and type classes
                        *t1_gen = *t2_gen;
                    }
                }
                else {
                    auto& ltimpl1 = std::get<LGeneric>(*t1_gen);
                    auto& ltimpl2 = std::get<LGeneric>(*t2_gen);

                    for (auto tc : ltimpl2) {
                        ltimpl1.insert(tc);
                    }
                }
            }*/
        }
    }


    std::cout << "type_id:" << std::endl; 
    for (auto& [x, y] : type_id) {
        std::cout << "(" << to_string(x) << ", " << y << ")" << std::endl;
    }
    std::cout << "type_sets:" << std::endl; 
    for (auto& [x, y] : type_sets) {
        std::string tmp = "";
        tmp += "(" + std::to_string(x) + ", {";
        for (auto& z : y) {
            tmp += to_string(z) + ", ";
        }
        if (!y.empty()) {
            tmp.pop_back();
            tmp.pop_back();
        }
        tmp += "})";
        std::cout << tmp << std::endl;
    }
    std::cout << "calls:" << std::endl; 
    for (auto& [x, y, z] : calls) {
        std::string tmp = "(" + to_string(x) + ", " + y + ", {";
        for (auto& w : z) {
            tmp += to_string(w) + ", ";
        }
        if (!z.empty()) {
            tmp.pop_back();
            tmp.pop_back();
        }
        tmp += "})";
        std::cout << tmp << std::endl;
    }
    std::cout << "\n\n\n";


}

std::map<PtrLType, size_t> LangTypeGraph::get_type_id() {
    return type_id;
}
