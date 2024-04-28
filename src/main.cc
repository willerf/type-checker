
#include <vector>
#include <cassert>

#include "type_check.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> input_file_paths;
    size_t i = 1;
    while (i < argc) {
        input_file_paths.push_back(argv[i]);
        i += 1;
    }
    
    type_check(input_file_paths);

    return 0;
}
