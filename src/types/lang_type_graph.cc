
#include "lang_type_graph.h"

#include <cassert>
#include <iostream>
#include "lang_type.h"
#include "lang_type_utils.h"

PtrLType LangTypeGraph::union_types(PtrLType t1, PtrLType t2) {
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
        }
    }
}

std::map<PtrLType, size_t> LangTypeGraph::get_type_id() {
    return type_id;
}
