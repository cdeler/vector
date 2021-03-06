CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(vector)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 14)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET(CMAKE_TESTS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-pointer-bool-conversion")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-pointer-bool-conversion")

SET(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

INCLUDE(${PROJECT_ROOT}/utils/GetIncludePath.cmake)
INCLUDE(${PROJECT_ROOT}/utils/Valgrind.cmake)
INCLUDE(${PROJECT_ROOT}/utils/CodeCoverage.cmake)

GET_INCLUDE_PATH(${PROJECT_ROOT} LIBRARY_INCLUDE_PATH)
INCLUDE_DIRECTORIES(${LIBRARY_INCLUDE_PATH})

FILE(GLOB LIBRARY_SOURCE_C_FILES "${PROJECT_ROOT}/src/*.c")
FILE(GLOB LIBRARY_SOURCE_CPP_FILES "${PROJECT_ROOT}/src/*.cpp")
FILE(GLOB LIBRARY_HEADER_FILES "${PROJECT_ROOT}/src/*.h")

SET(SOURCE_FILES ${LIBRARY_SOURCE_C_FILES} ${LIBRARY_SOURCE_CPP_FILES} ${LIBRARY_HEADER_FILES})

ADD_LIBRARY(vector STATIC ${SOURCE_FILES})

FILE(GLOB UNIT_TESTS_LIST "${PROJECT_ROOT}/src/tests/*unittest.c")

ENABLE_TESTING()

FOREACH (TEST_SOURCE_FILE_NAME ${UNIT_TESTS_LIST})
	FILE(RELATIVE_PATH TEST_SOURCE_NAME "${PROJECT_ROOT}/src/tests/" ${TEST_SOURCE_FILE_NAME})
	GET_FILENAME_COMPONENT(TEST_NAME ${TEST_SOURCE_NAME} NAME_WE)

	MESSAGE("Added ${TEST_NAME} test")
	ADD_EXECUTABLE(${TEST_NAME} ${TEST_SOURCE_FILE_NAME})

	SET_TARGET_PROPERTIES(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")
	TARGET_LINK_LIBRARIES(${TEST_NAME} vector cmocka)

	ADD_TEST(${TEST_NAME} ${CMAKE_TESTS_OUTPUT_DIRECTORY}/${TEST_NAME})
	ADD_VALGRIND_CHECK(valgrind ${TEST_NAME})
	SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES TIMEOUT 5)
ENDFOREACH (TEST_SOURCE_FILE_NAME)

