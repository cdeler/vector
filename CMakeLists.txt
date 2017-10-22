cmake_minimum_required(VERSION 2.8.4)

project(cmocker)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_TESTS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_custom_target(valgrind)

include(utils/CodeCoverage.cmake)
include(utils/Valgrind.cmake)

add_subdirectory(utils)

set(CMOCKER_SOURCE_FILES cmocker.c cmocker.h)
add_library(cmocker STATIC ${CMOCKER_SOURCE_FILES})
target_link_libraries(cmocker utils)

set(UNIT_TESTS_LIST unittest_mock_function.c)

enable_testing()

foreach(TEST_NAME ${UNIT_TESTS_LIST})
    MESSAGE("Added ${TEST_NAME} test")
    add_executable(${TEST_NAME} ${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")
    target_link_libraries(${TEST_NAME} cmocker utils)

    add_test(${TEST_NAME} ${CMAKE_TESTS_OUTPUT_DIRECTORY}/${TEST_NAME})
    add_valgrind_check(valgrind ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 5)
endforeach(TEST_NAME)

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
    SETUP_TARGET_FOR_COVERAGE(coverage ctest "coverage_data")
endif (CMAKE_BUILD_TYPE STREQUAL "Coverage")

