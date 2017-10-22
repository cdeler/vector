find_program(VALGRIND_PATH valgrind)

function(SETUP_VALGRIND_BUILD _targetname _testrunner)
    if(NOT VALGRIND_PATH)
        message(FATAL_ERROR "valgrind not found! Aborting...")
    endif(NOT VALGRIND_PATH)

    add_custom_target(${_targetname}
            ${VALGRIND_PATH} --leak-check=full ${_testrunner} --output-on-failure
            )

endfunction(SETUP_VALGRIND_BUILD _targetname _testrunner)