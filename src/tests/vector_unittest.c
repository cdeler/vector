//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "vector.h"

static void test_smoke()
	{
	Vector *v = vector_open();

	assert_non_null(v);
	assert_int_equal(0, vector_getLength(v));
	assert_memory_equal(v, VECTOR_EYECATCHER_VALUE, sizeof(VECTOR_EYECATCHER_VALUE));

	size_t rc = vector_close(&v);
	assert_null(v);
	assert_int_equal(0, rc);
	}

static void test_vector_add()
	{
	size_t rc;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, free);

	char *element = strdup("foo");
	vector_pushBack(pVector, element);

	assert_int_equal(1, vector_getLength(pVector));

	char *firstElement = vector_elementAt(pVector, 0);
	assert_string_equal("foo", firstElement);

	rc = vector_close(&pVector);
	assert_int_equal(1, rc);
	}

static void test_removeAt()
	{
	void *data;
	size_t i;
	char c;
	Vector *pVector = vector_open();

	vector_pushBack(pVector, "first");
	assert_int_equal(1, vector_getLength(pVector));
	data = vector_removeAt(pVector, 0);
	assert_non_null(data);
	assert_string_equal("first", data);
	assert_int_equal(0, vector_getLength(pVector));

	vector_pushBack(pVector, "first");
	vector_pushBack(pVector, "second");
	vector_pushBack(pVector, "third");
	assert_int_equal(3, vector_getLength(pVector));
	data = vector_removeAt(pVector, 1);
	assert_non_null(data);
	assert_string_equal("second", data);
	assert_int_equal(2, vector_getLength(pVector));
	assert_string_equal("first", vector_elementAt(pVector, 0));
	assert_string_equal("third", vector_elementAt(pVector, 1));

	assert_string_equal("third", vector_removeAt(pVector, 1));

	assert_int_equal(1, vector_getLength(pVector));
	assert_string_equal("first", vector_elementAt(pVector, 0));

	vector_close(&pVector);

	pVector = vector_open();
	for (c = '0'; c <= '9'; ++c)
		{
		vector_pushBack(pVector, (void *) (0ULL | c));
		}
	assert_int_equal(10, vector_getLength(pVector));

	assert_int_equal('9', (char) vector_removeAt(pVector, 9));
	static const char expectedVec1[] = "012345678";
	static const size_t expectedVec1Size = (sizeof(expectedVec1) / sizeof(char)) - 1;

	assert_int_equal(expectedVec1Size, vector_getLength(pVector));
	for (i = 0; i < expectedVec1Size; ++i)
		{
		assert_int_equal(expectedVec1[i], (char) vector_elementAt(pVector, i));
		}

	assert_int_equal('0', (char) vector_removeAt(pVector, 0));
	static const char expectedVec2[] = "12345678";
	static const size_t expectedVec2Size = (sizeof(expectedVec2) / sizeof(char)) - 1;

	assert_int_equal(expectedVec2Size, vector_getLength(pVector));
	for (i = 0; i < expectedVec2Size; ++i)
		{
		assert_int_equal(expectedVec2[i], (char) vector_elementAt(pVector, i));
		}

	assert_int_equal('4', (char) vector_removeAt(pVector, 3));

	static const char expectedVec3[] = "1235678";
	static const size_t expectedVec3Size = (sizeof(expectedVec3) / sizeof(char)) - 1;

	assert_int_equal(expectedVec3Size, vector_getLength(pVector));
	for (i = 0; i < expectedVec3Size; ++i)
		{
		assert_int_equal(expectedVec3[i], (char) vector_elementAt(pVector, i));
		}

	assert_null(vector_removeAt(pVector, 5555));
	assert_int_equal(7, vector_getLength(pVector));

	vector_close(&pVector);
	}

#define TEST5_VECTOR_SIZE (10000000ULL)

static void test_complex_with_multiple_realloc()
	{
	size_t i, val, N;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, NULL);

	for (i = 0; i < TEST5_VECTOR_SIZE; ++i)
		{
		vector_pushBack(pVector, (void *) i);
		}

	N = vector_getLength(pVector);
	assert_int_equal(TEST5_VECTOR_SIZE, N);

	for (i = 0; i < TEST5_VECTOR_SIZE; ++i)
		{
		val = (size_t) vector_elementAt(pVector, i);
		assert_int_equal(i, val);
		}

	val = vector_close(&pVector);

	assert_int_equal(TEST5_VECTOR_SIZE, val);
	}

static void test_insertAt()
	{
	size_t i;
	int rc;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, NULL);

	for (i = 0; i < 9; ++i)
		{
		vector_pushBack(pVector, (void *) i);
		}

	assert_int_equal(9, vector_getLength(pVector));

	rc = vector_insertAt(pVector, 9, (void *) 0xDEADBEEF);

	assert_int_equal(0, rc);

	static const size_t exectedArr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0xDEADBEEF};
	static const size_t exectedArr1Size = sizeof(exectedArr1) / sizeof(size_t);

	assert_int_equal(exectedArr1Size, vector_getLength(pVector));
	for (i = 0; i < exectedArr1Size; ++i)
		{
		assert_int_equal(exectedArr1[i], vector_elementAt(pVector, i));
		}

	rc = vector_insertAt(pVector, 5, (void *) 0xCAFEBABE);
	assert_int_equal(0, rc);

	static const size_t exectedArr2[] = {0, 1, 2, 3, 4, 0xCAFEBABE, 5, 6, 7, 8, 0xDEADBEEF};
	static const size_t exectedArr2Size = sizeof(exectedArr2) / sizeof(size_t);

	assert_int_equal(exectedArr2Size, vector_getLength(pVector));
	for (i = 0; i < exectedArr2Size; ++i)
		{
		assert_int_equal(exectedArr2[i], vector_elementAt(pVector, i));
		}

	rc = vector_insertAt(pVector, 0x100, (void *) 0x100500);

	assert_true(rc < 0);
	assert_int_equal(exectedArr2Size, vector_getLength(pVector));
	for (i = 0; i < exectedArr2Size; ++i)
		{
		assert_int_equal(exectedArr2[i], vector_elementAt(pVector, i));
		}

	vector_close(&pVector);
	}

int main()
	{
	static const struct CMUnitTest tests[] =
			{
					cmocka_unit_test(test_smoke),
					cmocka_unit_test(test_vector_add),
					cmocka_unit_test(test_removeAt),
					cmocka_unit_test(test_complex_with_multiple_realloc),
					cmocka_unit_test(test_insertAt),
			};

	return cmocka_run_group_tests_name("vector", tests, NULL, NULL);
	}