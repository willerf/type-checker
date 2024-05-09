
#pragma once
#include <string>
#include <variant>

enum class Terminal {
    BOFS,
    EOFS,
    FN,
    ID,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    LET,
    ASSIGN,
    SEMI,
    IF,
    ELSE,
    RET,
    OR,
    AND,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AMPERSAND,
    PIPE,
    PCT,
    NOT,
    NUM,
    TRUE,
    FALSE,
    STRLITERALNF,
    CHARLITERALNF,
    STRLITERAL,
    CHARLITERAL,
    ZERO,
    SPACE,
    CARRIAGERETURN,
    NEWLINE,
    TAB,
    COMMENT,
    START
};

enum class NonTerminal {
    s,
    fns,
    fn,
    optparams,
    params,
    vardef,
    stmtblock,
    stmts,
    stmt,
    expr,
    p1,
    p2,
    p3,
    p4,
    p5,
    p6,
    p7,
    p8,
    optargs,
    args
};

using State = std::variant<Terminal, NonTerminal>;

namespace state {
std::string to_string(Terminal terminal);
std::string to_string(NonTerminal non_terminal);
std::string to_string(State state);
}  // namespace state
