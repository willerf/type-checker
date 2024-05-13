
cmake_minimum_required(VERSION 3.21)
project(type-checker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(type_checker_lib STATIC
    src/exceptions/unreachable_error.cc
    src/lang/lang_parsing.cc
    src/lang/lang_scanning.cc
    src/lang/state.cc
    src/lex_parse/memo_map.cc
    src/lex_parse/parse_earley.cc
    src/lex_parse/parse_node.cc
    src/lex_parse/scanning.cc
    src/lex_parse/token.cc
    src/post_processing/extract_args.cc
    src/post_processing/extract_expr.cc
    src/post_processing/extract_fns.cc
    src/post_processing/extract_params.cc
    src/post_processing/extract_s.cc
    src/post_processing/extract_stmts.cc
    src/post_processing/extract_vardef.cc
    src/program_representation/array_access_node.cc
    src/program_representation/array_node.cc
    src/program_representation/assign_node.cc
    src/program_representation/binary_expr_node.cc
    src/program_representation/call_node.cc
    src/program_representation/fn_node.cc
    src/program_representation/if_node.cc
    src/program_representation/literal_node.cc
    src/program_representation/program_node.cc
    src/program_representation/ret_node.cc
    src/program_representation/stmt_block_node.cc
    src/program_representation/unary_expr_node.cc
    src/program_representation/var_access_node.cc
    src/program_representation/var_decl_node.cc
    src/program_representation/variable.cc
    src/program_representation/while_node.cc
    src/types/lang_type.cc
    src/types/lang_type_graph.cc
    src/types/lang_type_utils.cc
    src/utils/type_printer.cc
    src/visitors/eval_visitor.cc
    src/visitors/eval_visitor_utils.cc
    src/visitors/scope_vars_visitor.cc
    src/visitors/type_visitor.cc
)

target_include_directories(type_checker_lib PUBLIC
    src/exceptions
    src/lex_parse
    src/lang
    src/post_processing
    src/program_representation
    src/types
    src/utils
    src/visitors
    src
)

add_executable(type-check src/type_check_main.cc src/type_check.cc)
target_link_libraries(type-check type_checker_lib)

add_executable(execute src/execute_main.cc src/execute.cc)
target_link_libraries(execute type_checker_lib)

