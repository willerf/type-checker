
#include "stmt_block_node.h"
#include <memory>

StmtBlockNode::StmtBlockNode(std::vector<std::shared_ptr<ASTNode>> stmts) : stmts{stmts} {}

std::shared_ptr<StmtBlockNode> make_stmt_block(std::vector<std::shared_ptr<ASTNode>> stmts) {
    return std::make_shared<StmtBlockNode>(stmts);
}
