
#pragma once

#include <string>

class UnreachableError {
  public:
    const std::string file;
    const int line;
    const std::string func;
    UnreachableError(
        const std::string& file,
        int line,
        const std::string& func
    );
};

#define UNREACHABLE throw UnreachableError(__FILE__, __LINE__, __func__)
