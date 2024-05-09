#include "unreachable_error.h"

UnreachableError::UnreachableError(
    const std::string& file,
    int line,
    const std::string& func
) :
    file {file},
    line {line},
    func {func} {}
