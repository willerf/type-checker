
#include <memory>

struct Variable {
    std::shared_ptr<std::string> name;
    explicit Variable(const std::string& name);
};


