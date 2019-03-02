//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>

#include "vector.h"

static void test_smoke()
	{
	Vector *v = vector_open();

	assert(v);
	assert(vector_getLength(v) == 0);
	assert(!strcmp((void *) v, VECTOR_EYECATCHER_VALUE));

	size_t rc = vector_close(&v);
	assert(v == NULL);
	assert(rc == 0);
	}

static void test_vector_add()
	{
	size_t rc;
	Vector *pVector = vector_open();
	vector_set_deleter(pVector, free);

	char *element = strdup("foo");
	vector_pushBack(pVector, element);

	assert(vector_getLength(pVector) == 1ULL);

	char *firstElement = vector_elementAt(pVector, 0);
	assert(!strcmp(firstElement, "foo"));

	rc = vector_close(&pVector);
	assert(rc == 1);
	}

static void test_removeAt()
	{
	Vector *pVector = vector_open();

	vector_pushBack(pVector, "first");
	assert(vector_getLength(pVector) == 1);
	assert(vector_removeAt(pVector, 0) == 0);
	assert(vector_getLength(pVector) == 0);

	vector_pushBack(pVector, "first");
	vector_pushBack(pVector, "second");
	vector_pushBack(pVector, "third");
	assert(vector_getLength(pVector) == 3);
	assert(vector_removeAt(pVector, 1) == 0);
	assert(vector_getLength(pVector) == 2);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));
	assert(!strcmp(vector_elementAt(pVector, 1), "third"));

	assert(vector_removeAt(pVector, 1) == 0);
	assert(vector_getLength(pVector) == 1);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));

	vector_close(&pVector);

	pVector = vector_open();
	vector_set_deleter(pVector, free);
	char c;
	size_t i;
	char buffer[2] = "X";
	for (c = '0'; c <= '9'; ++c)
		{
		*buffer = c;
		vector_pushBack(pVector, strdup(buffer));
		}
	assert(vector_getLength(pVector) == 10);

	assert(vector_removeAt(pVector, 9) == 0);
	assert(vector_getLength(pVector) == 9);
	for (c = '0', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 0) == 0);
	assert(vector_getLength(pVector) == 8);
	for (c = '1', i = 0; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 3) == 0);
	assert(vector_getLength(pVector) == 7);
	for (c = '1', i = 0; c <= '3'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}
	for (c = '5'; c <= '8'; ++c, ++i)
		{
		*buffer = c;
		assert(!strcmp(vector_elementAt(pVector, i), buffer));
		}

	assert(vector_removeAt(pVector, 5555) < 0);
	assert(vector_getLength(pVector) == 7);

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
	assert(TEST5_VECTOR_SIZE == N);

	for (i = 0; i < TEST5_VECTOR_SIZE; ++i)
		{
		val = (size_t) vector_elementAt(pVector, i);
		assert(val == i);
		}

	val = vector_close(&pVector);

	assert(val == TEST5_VECTOR_SIZE);
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

	assert(vector_getLength(pVector) == 9);

	rc = vector_insertAt(pVector, 9, (void *) 0xDEADBEEF);

	assert(rc == 0);

	static const size_t exectedArr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 0xDEADBEEF};
	static const size_t exectedArr1Size = sizeof(exectedArr1) / sizeof(size_t);
	assert(vector_getLength(pVector) == exectedArr1Size);

	for (i = 0; i < exectedArr1Size; ++i)
		{
		assert(vector_elementAt(pVector, i) == (void *) exectedArr1[i]);
		}

	rc = vector_insertAt(pVector, 5, (void *) 0xCAFEBABE);
	assert(rc == 0);

	static const size_t exectedArr2[] = {0, 1, 2, 3, 4, 0xCAFEBABE, 5, 6, 7, 8, 0xDEADBEEF};
	static const size_t exectedArr2Size = sizeof(exectedArr2) / sizeof(size_t);
	assert(vector_getLength(pVector) == exectedArr2Size);

	for (i = 0; i < exectedArr2Size; ++i)
		{
		assert(vector_elementAt(pVector, i) == (void *) exectedArr2[i]);
		}

	rc = vector_insertAt(pVector, 0x100, (void *) 0x100500);
	assert(rc < 0);

	assert(vector_getLength(pVector) == exectedArr2Size);
	for (i = 0; i < exectedArr2Size; ++i)
		{
		assert(vector_elementAt(pVector, i) == (void *) exectedArr2[i]);
		}


	vector_close(&pVector);
	}

int main()
	{
	test_smoke();
	test_vector_add();
	test_removeAt();
	test_complex_with_multiple_realloc();
	test_insertAt();

	return 0;
	}