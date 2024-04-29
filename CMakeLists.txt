
cmake_minimum_required(VERSION 3.21)
project(type-checker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(type_checker_lib STATIC
    src/lang/lang_parsing.cc
    src/lang/lang_scanning.cc
    src/lang/state.cc
    src/lex_parse/memo_map.cc
    src/lex_parse/parse_earley.cc
    src/lex_parse/parse_node.cc
    src/lex_parse/scanning.cc
    src/lex_parse/token.cc
    src/post_processing/extract_fns.cc
    src/program_representation/stmt_block_node.cc
    src/program_representation/fn_node.cc
    src/program_representation/variable.cc
    src/transformations/visitor.cc
    src/type_check.cc
)

target_include_directories(type_checker_lib PUBLIC
    src/lex_parse
    src/lang
    src/post_processing
    src/program_representation
    src/transformations
    src
)

add_executable(typecheck src/main.cc)
target_link_libraries(typecheck type_checker_lib)

