
#include "lang_type_graph.h"

#include <cassert>
#include <iostream>

#include "lang_type.h"
#include "lang_type_utils.h"
#include "unreachable_error.h"

PtrLType LangTypeGraph::union_types(PtrLType ptr_t1, PtrLType ptr_t2) {
    assert(type_id.contains(ptr_t1));
    assert(type_id.contains(ptr_t2));

    size_t tid1 = type_id.at(ptr_t1);
    size_t tid2 = type_id.at(ptr_t2);

    PtrLType result_type = nullptr;

    std::visit(
        overloaded {
            [&](const LPrim& t1, const LPrim& t2) {
                if (t1 == t2) {
                    if (type_sets.at(tid1).size() > type_sets.at(tid2).size()) {
                        result_type = ptr_t1;
                    } else {
                        result_type = ptr_t2;
                    }
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](const LPrim& t1, const LGeneric& t2) {
                if (compatible(t1, t2)) {
                    result_type = ptr_t1;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](const LGeneric& t1, const LPrim& t2) {
                if (compatible(t2, t1)) {
                    result_type = ptr_t2;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](const LGeneric& t1, const LArray& t2) {
                if (compatible(t2, t1)) {
                    result_type = ptr_t2;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](const LGeneric& t1, const LGeneric& t2) {
                LGeneric tcs = {};
                std::set_union(
                    t1.begin(),
                    t1.end(),
                    t2.begin(),
                    t2.end(),
                    std::inserter(tcs, tcs.begin())
                );
                result_type = make_lt(tcs);
            },
            [&](const LArray& t1, const LGeneric& t2) {
                if (compatible(t1, t2)) {
                    result_type = ptr_t1;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](const LArray& t1, const LArray& t2) {
                auto elem_type = union_types(t1.ltype, t2.ltype);
                result_type = make_lt(LArray {elem_type});
            },
            [&](const auto&, const auto&) {
                throw TypeError(**ptr_t1, **ptr_t2);
            },
        },
        **ptr_t1,
        **ptr_t2
    );

    assert(result_type);

    if (result_type == ptr_t1) {
        for (auto ptr_ltype : type_sets.at(tid2)) {
            *ptr_ltype = *result_type;
            type_id[ptr_ltype] = tid1;
            type_sets[tid1].insert(ptr_ltype);
        }
    } else if (result_type == ptr_t2) {
        for (auto ptr_ltype : type_sets.at(tid1)) {
            *ptr_ltype = *result_type;
            type_id[ptr_ltype] = tid2;
            type_sets[tid2].insert(ptr_ltype);
        }
    } else {
        size_t result_tid = count++;
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
    }
    return result_type;
}

PtrLType LangTypeGraph::add_tc(PtrLType ptr_ltype, LTypeClass tc) {
    assert(type_id.contains(ptr_ltype));

    std::visit(
        overloaded {
            [&](LPrim& lprim) {
                if (!compatible(lprim, LGeneric {tc})) {
                    throw TypeError(**ptr_ltype, LGeneric {tc});
                }
            },
            [&](LGeneric& lgeneric) { lgeneric.insert(tc); },
            [&](LArray& larray) {
                if (!compatible(larray, LGeneric {tc})) {
                    throw TypeError(**ptr_ltype, LGeneric {tc});
                }
            }},
        **ptr_ltype
    );

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

PtrLType
LangTypeGraph::add_call(std::string fn_name, std::vector<PtrLType> arg_types) {
    auto ptr_ltype = add_type(make_lt(LGeneric {}));
    calls.push_back({ptr_ltype, fn_name, arg_types});
    return ptr_ltype;
}

void LangTypeGraph::add_fn(std::string fn_name, std::shared_ptr<FnNode> fn) {
    fn_map[fn_name] = fn;
}

bool LangTypeGraph::sub_type(PtrLType ptr_t1, PtrLType ptr_t2) {
    bool modified = false;
    std::visit(
        overloaded {
            [&](LPrim& t1, LPrim& t2) {
                if (t1 != t2) {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](LPrim& t1, LGeneric& t2) {
                if (!compatible(t1, t2)) {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](LGeneric& t1, LPrim& t2) {
                if (compatible(t2, t1)) {
                    **ptr_t1 = **ptr_t2;
                    modified = true;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](LGeneric& t1, LArray& t2) {
                if (compatible(t2, t1)) {
                    **ptr_t1 = **ptr_t2;
                    modified = true;
                } else {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](LGeneric& t1, LGeneric& t2) {
                size_t t1_size = t1.size();
                t1.insert(t2.begin(), t2.end());
                if (t1_size != t1.size()) {
                    modified = true;
                }
            },
            [&](LArray& t1, LGeneric& t2) {
                if (!compatible(t1, t2)) {
                    throw TypeError(**ptr_t1, **ptr_t2);
                }
            },
            [&](LArray& t1, LArray& t2) { sub_type(t1.ltype, t2.ltype); },
            [&](auto&, auto&) { throw TypeError(**ptr_t1, **ptr_t2); },
        },
        **ptr_t1,
        **ptr_t2
    );
    return modified;
}

bool LangTypeGraph::union_types_mod(PtrLType ptr_t1, PtrLType ptr_t2) {
    if (type_id.at(ptr_t1) == type_id.at(ptr_t2)) {
        return false;
    }
    union_types(ptr_t1, ptr_t2);
    return true;
}

void LangTypeGraph::reduce() {
    int n = calls.size();
    bool modified;

    do {
        modified = false;
        for (int i = 0; i < n; i++) {
            auto& [ret_type, fn_name, arg_types] = calls[i];
            auto fn = fn_map.at(fn_name);
            assert(fn->params.size() == arg_types.size());

            for (int i = 0; i < fn->params.size(); i++) {
                if (type_id[fn->params[i].impl->ptr_ltype]
                    == type_id[fn->ret_type]) {
                    modified |= union_types_mod(ret_type, arg_types[i]);
                }
            }

            for (int i = 0; i < fn->params.size(); i++) {
                auto p1 = fn->params[i].impl->ptr_ltype;
                for (int j = i + 1; j < fn->params.size(); j++) {
                    auto p2 = fn->params[j].impl->ptr_ltype;
                    if (type_id.at(p1) == type_id.at(p2)) {
                        modified |= union_types_mod(arg_types[i], arg_types[j]);
                    }
                }
            }
        }
    } while (modified);

    for (auto& [_, fn] : fn_map) {
        if (type_sets.at(type_id.at(fn->ret_type)).size() == 1
            && std::holds_alternative<LGeneric>(**fn->ret_type)) {
            **fn->ret_type = LPrim::Unit;
        }
    }

    do {
        modified = false;
        for (int j = 0; j < n; j++) {
            auto& [ret_type, fn_name, arg_types] = calls[j];
            auto fn = fn_map.at(fn_name);

            modified |= sub_type(ret_type, fn->ret_type);

            for (int k = 0; k < fn->params.size(); k++) {
                auto param_type = fn->params[k].impl->ptr_ltype;
                modified |= sub_type(arg_types[k], param_type);
            }
        }
    } while (modified);
}
