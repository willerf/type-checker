
#include "variable.h"

#include <memory>
#include <string>

VarImpl::VarImpl(const std::string& name, PtrLType ptr_ltype) :
    name {name},
    ptr_ltype {ptr_ltype} {}

Variable::Variable(const std::string& name, LTypeImpl ltypeimpl) {
    impl = std::make_shared<VarImpl>(name, make_lt(ltypeimpl));
}
