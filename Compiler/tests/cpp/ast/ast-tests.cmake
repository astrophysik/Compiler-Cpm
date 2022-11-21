PREPEND(AST_TESTS_SOURCES "${PROJECT_SOURCE_DIR}/tests/cpp/ast" "simple-ast-tests.cpp")
include_directories(${PROJECT_SOURCE_DIR})
add_unittest(ast ${AST_TESTS_SOURCES})