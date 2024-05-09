
#pragma once

#include <string_view>
#include <vector>

#include "dfa.h"
#include "token.h"

struct DFA;

Token scan_one(std::string_view input, DFA& dfa);
std::vector<Token> maximal_munch_scan(std::string_view input, DFA& dfa);

class ScanError {
public:
    const size_t line_no;
    ScanError(size_t line_no);
};
