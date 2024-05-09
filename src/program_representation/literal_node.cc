
#include "literal_node.h"

LiteralNode::LiteralNode(LiteralType literal_type, std::string value) :
    literal_type {literal_type},
    value {value} {}

std::shared_ptr<LiteralNode>
make_literal(LiteralType literal_type, std::string value) {
    return std::make_shared<LiteralNode>(literal_type, value);
}
