FIND_PROGRAM(VALGRIND_PATH valgrind)

FUNCTION(ADD_VALGRIND_CHECK _targetname _test_name)
	IF (NOT VALGRIND_PATH)
		MESSAGE(FATAL_ERROR "valgrind not found! Aborting...")
	ENDIF (NOT VALGRIND_PATH)

	ADD_CUSTOM_TARGET(${_test_name}_valgrind
			${VALGRIND_PATH} --leak-check=full --show-reachable=yes --error-exitcode=1 $<TARGET_FILE:${_test_name}>
			)
	ADD_DEPENDENCIES(valgrind ${_test_name}_valgrind)
ENDFUNCTION(ADD_VALGRIND_CHECK _targetname _testrunner)