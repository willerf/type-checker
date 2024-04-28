#include "lang_parsing.h"

#include <iostream>
#include <map>
#include <optional>
#include <variant>
#include <vector>

#include "parse_earley.h"
#include "state.h"

struct Token;

static const std::map<NonTerminal, std::vector<Production>> productions = {
    {NonTerminal::s, 
        {{NonTerminal::s, {Terminal::BOFS, NonTerminal::fns, Terminal::EOFS}}}
    },
    {NonTerminal::fns, 
        {{NonTerminal::fns, {NonTerminal::fn, NonTerminal::fns}}, 
        {NonTerminal::fns, {NonTerminal::fn}}}
    },
    {NonTerminal::fn, 
        {{NonTerminal::fn, {Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}}}
    },
    {NonTerminal::optparams,
        {{NonTerminal::optparams, {NonTerminal::params}},
        {NonTerminal::optparams, {}}}
    },
    {NonTerminal::params,
        {{NonTerminal::params, {NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}},
        {NonTerminal::params, {NonTerminal::vardef}}}
    },
    {NonTerminal::vardef,
        {{NonTerminal::vardef, {Terminal::ID}}}
    },
    {NonTerminal::stmtblock,
        {{NonTerminal::stmtblock, {Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}}}
    },
    {NonTerminal::stmts,
        {{NonTerminal::stmts, {NonTerminal::stmt, NonTerminal::stmts}},
        {NonTerminal::stmts, {}}}
    },
    {NonTerminal::stmt,
        {{NonTerminal::stmt, {Terminal::LET, NonTerminal::vardef, Terminal::ASSIGN, NonTerminal::expr, Terminal::SEMI}},
        {NonTerminal::stmt, {Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock, Terminal::ELSE, NonTerminal::stmtblock}},
        {NonTerminal::stmt, {Terminal::RET, NonTerminal::expr, Terminal::SEMI}}}
    },
    {NonTerminal::expr, {{NonTerminal::expr, {NonTerminal::p1}}}},
    {NonTerminal::p1,
     {{NonTerminal::p1, {NonTerminal::p2}},
      {NonTerminal::p1,
       {NonTerminal::p1, Terminal::OR, NonTerminal::p2}}}},
    {NonTerminal::p2,
     {{NonTerminal::p2, {NonTerminal::p3}},
      {NonTerminal::p2,
       {NonTerminal::p2, Terminal::AND, NonTerminal::p3}}}},
    {NonTerminal::p3,
     {{NonTerminal::p3, {NonTerminal::p4}},
      {NonTerminal::p3,
       {NonTerminal::p3, Terminal::EQ, NonTerminal::p4}},
      {NonTerminal::p3,
       {NonTerminal::p3, Terminal::NE, NonTerminal::p4}}}},
    {NonTerminal::p4,
     {{NonTerminal::p4, {NonTerminal::p5}},
      {NonTerminal::p4,
       {NonTerminal::p4, Terminal::LT, NonTerminal::p5}},
      {NonTerminal::p4,
       {NonTerminal::p4, Terminal::GT, NonTerminal::p5}},
      {NonTerminal::p4,
       {NonTerminal::p4, Terminal::LE, NonTerminal::p5}},
      {NonTerminal::p4,
       {NonTerminal::p4, Terminal::GE, NonTerminal::p5}}}},
    {NonTerminal::p5,
     {{NonTerminal::p5, {NonTerminal::p6}},
      {NonTerminal::p5,
       {NonTerminal::p5, Terminal::PLUS, NonTerminal::p6}},
      {NonTerminal::p5,
       {NonTerminal::p5, Terminal::MINUS, NonTerminal::p6}}}},
    {NonTerminal::p6,
     {{NonTerminal::p6, {NonTerminal::p7}},
      {NonTerminal::p6,
       {NonTerminal::p6, Terminal::STAR, NonTerminal::p7}},
      {NonTerminal::p6,
       {NonTerminal::p6, Terminal::SLASH, NonTerminal::p7}},
      {NonTerminal::p6,
       {NonTerminal::p6, Terminal::PCT, NonTerminal::p7}}}},
    {NonTerminal::p7,
     {{NonTerminal::p7, {NonTerminal::p8}},
      {NonTerminal::p7, {Terminal::NOT, NonTerminal::p8}}}},
    {NonTerminal::p8,
     {
         {NonTerminal::p8, {Terminal::ID}},
         {NonTerminal::p8, {Terminal::NUM}},
         {NonTerminal::p8, {Terminal::TRUE}},
         {NonTerminal::p8, {Terminal::FALSE}},
         {NonTerminal::p8,
          {Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}},
         {NonTerminal::p8,
          {Terminal::ID,
           Terminal::LPAREN,
           NonTerminal::optargs,
           Terminal::RPAREN}},
     }},
    {NonTerminal::optargs,
     {{NonTerminal::optargs, {NonTerminal::args}}, {NonTerminal::optargs, {}}}},
    {NonTerminal::args,
     {{NonTerminal::args,
       {NonTerminal::expr, Terminal::COMMA, NonTerminal::args}},
      {NonTerminal::args, {NonTerminal::expr}}}}
    };

static const Grammar nex_lang_grammar {
    .start = NonTerminal::s,
    .productions = productions,
};

Grammar make_nex_lang_grammar() {
    return nex_lang_grammar;
}

ParseNode parse(std::span<Token> input) {
    Grammar grammar = nex_lang_grammar;
    std::optional<ParseNode> result = parse_earley(input, grammar);
    if (result) {
        return result.value();
    }
    std::cerr << "Unknown parsing error" << std::endl;
    exit(1);
}
