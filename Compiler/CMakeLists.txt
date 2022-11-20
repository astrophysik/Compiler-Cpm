cmake_minimum_required(VERSION 3.23)
project(Compiler-Cpm)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads)

add_executable(cpm src/main.cpp AST/ast_nodes.cpp helpers/char_source.cpp src/lexer.cpp src/parser.cpp src/translator.cpp src/compiler.cpp)
target_link_libraries (cpm ${CMAKE_THREAD_LIBS_INIT})