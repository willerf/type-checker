
#include "variable.h"
#include <memory>
#include <string>

VarImpl::VarImpl(std::string name, std::variant<LPrim, LCustom> type_info) : name{name}, type_info{type_info} {}
Variable::Variable(const std::string& name, std::variant<LPrim, LCustom> type_info) {
    impl = std::make_shared<VarImpl>(name, type_info);
}
