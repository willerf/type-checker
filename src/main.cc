
#include <cassert>
#include <iostream>
#include <vector>

#include "lang_type.h"
#include "parse_earley.h"
#include "scanning.h"
#include "scope_vars_visitor.h"
#include "type_check.h"
#include "unreachable_error.h"

int main(int argc, char* argv[]) {
    std::vector<std::string> input_file_paths;
    size_t i = 1;
    while (i < argc) {
        input_file_paths.push_back(argv[i]);
        i += 1;
    }

    try {
        type_check(input_file_paths);
    } catch (const ScanError& e) {
        std::cerr << "ERROR: Scanning error on line: " << e.line_no
                  << std::endl;
    } catch (const ParseError& e) {
        std::cerr << "ERROR: Parsing error on line: " << e.line_no << std::endl;
    } catch (const VariableNotFoundError& e) {
        std::cerr << "ERROR: Unknown variable `" << e.name
                  << "` on line: " << e.line_no << std::endl;
    } catch (const TypeError& e) {
        std::cerr << "ERROR: Incompatible types `" << to_string(e.t1)
                  << "` and `" << to_string(e.t2) << "`" << std::endl;
    } catch (const UnreachableError& e) {
        std::cerr << e.file << ":" << e.line << " " << e.func << std::endl;
        throw;
    }

    return 0;
}
