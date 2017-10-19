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


set(CMOCKER_SOURCE_FILES cmocker.c cmocker.h)

add_library(cmocker STATIC ${CMOCKER_SOURCE_FILES})

add_custom_target(build_tests)

set(UNIT_TESTS_LIST unittest_mock_function.c)

enable_testing()

foreach(TEST_NAME ${UNIT_TESTS_LIST})
    MESSAGE("Added ${TEST_NAME} test")
    add_executable(${TEST_NAME} ${TEST_NAME})

    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")
    target_link_libraries(${TEST_NAME} cmocker)

    add_test(${TEST_NAME} ${CMAKE_TESTS_OUTPUT_DIRECTORY}/${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 5)

    add_dependencies(build_tests ${TEST_NAME})
endforeach(TEST_NAME)
