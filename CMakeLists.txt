
cmake_minimum_required(VERSION 3.21)
project(type-checker)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(type_checker_lib STATIC
    src/lex_parse/token_type.cc
    src/lex_parse/scanning.cc
)

target_include_directories(type_checker_lib PUBLIC
    src
    src/lex_parse
)

add_executable(typecheck src/main.cc)
target_link_libraries(typecheck type_checker_lib)

