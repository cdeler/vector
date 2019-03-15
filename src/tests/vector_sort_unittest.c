//
// Created by sergei_krotov on 3/15/19.
//


#include <assert.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "vector.h"

static Vector *
_get_range(int64_t start, int64_t stop)
	{
	Vector *result = vector_open();

	if (start <= stop)
		{
		while (start <= stop)
			{
			vector_pushBack(result, (void *) start);
			++start;
			}
		}
	else
		{
		while (start >= stop)
			{
			vector_pushBack(result, (void *) start);
			--start;
			}
		}

	return result;
	}

static int
_uint_cmp(const void *i1, const void *i2)
	{
	return (int) ((int64_t) i1 - (int64_t) i2);
	}

static void
test_sort_already_sorted()
	{
	Vector *data;
	int64_t i;

	data = _get_range(0, 100);

	vector_sort(data, _uint_cmp);

	for (i = 0; i <= 100; ++i)
		{
		assert_int_equal(i, vector_elementAt(data, i));
		}

	vector_close(&data);
	}


static void
test_sort_reversed()
	{
	Vector *data;
	int64_t i;

	data = _get_range(100, 0);

	vector_sort(data, _uint_cmp);

	for (i = 0; i <= 100; ++i)
		{
		assert_int_equal(100 - i, vector_elementAt(data, i));
		}

	vector_close(&data);
	}

int main()
	{
	static const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test_sort_already_sorted),
					cmocka_unit_test(test_sort_reversed),
			};

	return cmocka_run_group_tests_name("vector_sort", tests, NULL, NULL);
	}