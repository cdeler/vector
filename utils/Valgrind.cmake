find_program(VALGRIND_PATH valgrind)

function(add_valgrind_check _targetname _test_name)
    if(NOT VALGRIND_PATH)
        message(FATAL_ERROR "valgrind not found! Aborting...")
    endif(NOT VALGRIND_PATH)

    add_custom_target(${_test_name}_valgrind
            ${VALGRIND_PATH} --leak-check=full --show-reachable=yes --error-exitcode=1 $<TARGET_FILE:${_test_name}>
            )
    add_dependencies(valgrind ${_test_name}_valgrind)
endfunction(add_valgrind_check _targetname _testrunner)