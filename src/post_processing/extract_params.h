
#pragma once

#include <memory>
#include <vector>

#include "parse_node.h"
#include "variable.h"

std::vector<Variable> extract_params(ParseNode root);

std::vector<Variable> extract_optparams(ParseNode root);
