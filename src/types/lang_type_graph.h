#pragma once

#include <map>
#include <set>

#include "fn_node.h"
#include "lang_type.h"

class LangTypeGraph {
    size_t count = 0;
    std::map<PtrLType, size_t> type_id;
    std::map<size_t, std::set<PtrLType>> type_sets;
    std::vector<std::tuple<PtrLType, std::string, std::vector<PtrLType>>> calls;
    std::map<std::string, std::shared_ptr<FnNode>> fn_map;

  public:
    PtrLType union_types(PtrLType t1, PtrLType t2);
    PtrLType add_tc(PtrLType ptr_ltype, LTypeClass tc);
    PtrLType add_type(PtrLType ptr_ltype);
    PtrLType add_call(std::string fn_name, std::vector<PtrLType> arg_types);
    void add_fn(std::string fn_name, std::shared_ptr<FnNode> fn);
    void reduce();
};
