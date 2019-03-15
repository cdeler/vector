//
// Created by sergei_krotov on 3/7/19.
//


#include <assert.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "vector.h"

static Vector *_get_range(size_t start, size_t stop)
	{
	Vector *result = vector_open();

	while (start < stop)
		{
		vector_pushBack(result, (void *) start);
		++start;
		}

	return result;
	}

static Vector *_get_state(void **state, size_t idx)
	{
	return ((Vector **) *state)[idx];
	}

static void _set_state(void **state, void *val, size_t idx)
	{
	((Vector **) *state)[idx] = val;
	}

static int _setup(void **testState)
	{
	*testState = calloc(2, sizeof(Vector *));

	_set_state(testState, _get_range(0, 10), 0);
	_set_state(testState, _get_range(10, 100), 1);

	return 0;
	}

static int _teardown(void **testState)
	{
	Vector *val;

	val = _get_state(testState, 0);
	vector_close(&val);

	val = _get_state(testState, 1);
	vector_close(&val);

	free(*testState);

	*testState = NULL;

	return 0;
	}

static void test_null_null_append()
	{
	Vector *res;

	res = vector_concat(NULL, NULL);
	assert_non_null(res);
	assert_int_equal(0, vector_getLength(res));
	vector_close(&res);
	}

static void test_null_val_append(void **state)
	{
	int i, j;
	Vector *second, *res;

	second = _get_state(state, 1);
	res = vector_concat(NULL, second);
	assert_non_null(res);
	assert_int_equal(90, vector_getLength(res));
	for (i = 10, j = 0; j < 90; ++i, ++j)
		{
		assert_int_equal(i, vector_elementAt(res, j));
		}
	vector_close(&res);
	}

static void test_val_null(void **state)
	{
	int i;
	Vector *first, *res;

	first = _get_state(state, 0);

	res = vector_concat(first, NULL);
	assert_non_null(res);
	assert_int_equal(10, vector_getLength(res));
	for (i = 0; i < 10; ++i)
		{
		assert_int_equal(i, vector_elementAt(res, i));
		}
	vector_close(&res);

	_set_state(state, res, 0);
	}

static void test_val_val(void **state)
	{
	size_t i;
	Vector *first, *second, *res;

	first = _get_state(state, 0);
	second = _get_state(state, 1);

	res = vector_concat(first, second);

	assert_non_null(res);
	assert_ptr_equal(first, res);
	assert_int_equal(100, vector_getLength(res));
	for (i = 0; i < 100; ++i)
		{
		assert_int_equal(i, vector_elementAt(res, i));
		}
	vector_close(&res);

	_set_state(state, res, 0);
	}

int main()
	{
	static const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test_null_null_append),
					cmocka_unit_test_setup_teardown(test_null_val_append, _setup, _teardown),
					cmocka_unit_test_setup_teardown(test_val_null, _setup, _teardown),
					cmocka_unit_test_setup_teardown(test_val_val, _setup, _teardown),
			};

	return cmocka_run_group_tests_name("vector_concat", tests, NULL, NULL);
	}