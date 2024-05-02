
#include "variable.h"
#include <memory>
#include <string>

VarImpl::VarImpl(const std::string& name, std::shared_ptr<LType> type_info) : name{name}, type_info{type_info} {}
Variable::Variable(const std::string& name, LType type_info) {
    impl = std::make_shared<VarImpl>(name, std::make_shared<LType>(type_info));
}
