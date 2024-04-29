
#include "variable.h"
#include <memory>
#include <string>

Variable::Variable(const std::string& name) {
    this->name = std::make_shared<std::string>(name);
}

