function(PREPEND var prefix)
    set(listVar "")
    foreach(f ${ARGN})
        list(APPEND listVar "${prefix}/${f}")
    endforeach(f)
    set(${var} "${listVar}" PARENT_SCOPE)
endfunction(PREPEND)

enable_testing()

function(add_unittest TEST_NAME)
    add_executable(${TEST_NAME} ${COMPILER_SRC} ${ARGN})
    target_link_libraries(${TEST_NAME} GTest::gtest_main)
    gtest_discover_tests(${TEST_NAME})
endfunction()

include(tests/cpp/ast/ast-tests.cmake)
include(tests/cpp/runtime/runtime-tests.cmake)
