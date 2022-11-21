cmake_minimum_required(VERSION 3.16)
project(Compiler-Cpm)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(COMPILER_SRC AST/ast_nodes.cpp helpers/char_source.cpp src/lexer.cpp src/parser.cpp src/translator.cpp src/compiler.cpp)

include(GoogleTest)
include(tests/tests.cmake)

add_executable(cpm ${COMPILER_SRC} src/main.cpp)
target_link_libraries (cpm ${CMAKE_THREAD_LIBS_INIT})