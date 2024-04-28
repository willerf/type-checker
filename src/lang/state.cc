
#include "state.h"

#include <string>

std::string state::to_string(Terminal terminal) {
    switch (terminal) {
        case Terminal::BOFS:
            return "BOFS";
        case Terminal::EOFS:
            return "EOFS";
        case Terminal::FN:
            return "FN";
        case Terminal::ID:
            return "ID";
        case Terminal::LPAREN:
            return "LPAREN";
        case Terminal::RPAREN:
            return "RPAREN";
        case Terminal::LBRACE:
            return "LBRACE";
        case Terminal::RBRACE:
            return "RBRACE";
        case Terminal::COMMA:
            return "COMMA";
        case Terminal::TRUE:
            return "TRUE";
        case Terminal::FALSE:
            return "FALSE";
        case Terminal::LET:
            return "LET";
        case Terminal::ASSIGN:
            return "ASSIGN";
        case Terminal::SEMI:
            return "SEMI";
        case Terminal::IF:
            return "IF";
        case Terminal::ELSE:
            return "ELSE";
        case Terminal::RET:
            return "RET";
        case Terminal::OR:
            return "OR";
        case Terminal::AND:
            return "AND";
        case Terminal::EQ:
            return "EQ";
        case Terminal::NE:
            return "NE";
        case Terminal::LT:
            return "LT";
        case Terminal::GT:
            return "GT";
        case Terminal::LE:
            return "LE";
        case Terminal::GE:
            return "GE";
        case Terminal::PLUS:
            return "PLUS";
        case Terminal::MINUS:
            return "MINUS";
        case Terminal::STAR:
            return "STAR";
        case Terminal::SLASH:
            return "SLASH";
        case Terminal::AMPERSAND:
            return "AMPERSAND";
        case Terminal::PIPE:
            return "PIPE";
        case Terminal::PCT:
            return "PCT";
        case Terminal::NOT:
            return "NOT";
        case Terminal::NUM:
            return "NUM";
        case Terminal::ZERO:
            return "ZERO";
        case Terminal::SPACE:
            return "SPACE";
        case Terminal::CARRIAGERETURN:
            return "CARRIAGERETURN";
        case Terminal::NEWLINE:
            return "NEWLINE";
        case Terminal::TAB:
            return "TAB";
        case Terminal::COMMENT:
            return "COMMENT";
        case Terminal::START:
            return "START";
        default:
            return "UNKNOWN";
    }
}

std::string state::to_string(NonTerminal non_terminal) {
    switch (non_terminal) {
        case NonTerminal::s:
            return "s";
        case NonTerminal::fns:
            return "fns";
        case NonTerminal::fn:
            return "fn";
        case NonTerminal::optparams:
            return "optparams";
        case NonTerminal::params:
            return "params";
        case NonTerminal::vardef:
            return "vardef";
        case NonTerminal::stmtblock:
            return "stmtblock";
        case NonTerminal::stmts:
            return "stmts";
        case NonTerminal::stmt:
            return "stmt";
        case NonTerminal::expr:
            return "expr";
        case NonTerminal::p1:
            return "p1";
        case NonTerminal::p2:
            return "p2";
        case NonTerminal::p3:
            return "p3";
        case NonTerminal::p4:
            return "p4";
        case NonTerminal::p5:
            return "p5";
        case NonTerminal::p6:
            return "p6";
        case NonTerminal::p7:
            return "p7";
        case NonTerminal::p8:
            return "p8";
        case NonTerminal::optargs:
            return "optargs";
        case NonTerminal::args:
            return "args";
        default:
            return "UNKNOWN";
    }
}

std::string state::to_string(State state) {
    if (std::holds_alternative<Terminal>(state)) {
        return to_string(std::get<Terminal>(state));
    }
    if (std::holds_alternative<NonTerminal>(state)) {
        return to_string(std::get<NonTerminal>(state));
    }
    return "UNKNOWN";
}
