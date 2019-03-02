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
	void *data;
	size_t i;
	char c;
	Vector *pVector = vector_open();

	vector_pushBack(pVector, "first");
	assert(vector_getLength(pVector) == 1);
	data = vector_removeAt(pVector, 0);
	assert(data);
	assert(!strcmp(data, "first"));
	assert(vector_getLength(pVector) == 0);

	vector_pushBack(pVector, "first");
	vector_pushBack(pVector, "second");
	vector_pushBack(pVector, "third");
	assert(vector_getLength(pVector) == 3);
	data = vector_removeAt(pVector, 1);
	assert(data);
	assert(!strcmp(data, "second"));
	assert(vector_getLength(pVector) == 2);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));
	assert(!strcmp(vector_elementAt(pVector, 1), "third"));

	assert(!strcmp(vector_removeAt(pVector, 1), "third"));

	assert(vector_getLength(pVector) == 1);
	assert(!strcmp(vector_elementAt(pVector, 0), "first"));

	vector_close(&pVector);

	pVector = vector_open();
	for (c = '0'; c <= '9'; ++c)
		{
		vector_pushBack(pVector, (void *) c);
		}
	assert(vector_getLength(pVector) == 10);

	c = (char) vector_removeAt(pVector, 9);
	assert(c == '9');
	static const char expectedVec1[] = "012345678";
	static const size_t expectedVec1Size = (sizeof(expectedVec1) / sizeof(char)) - 1;

	assert(vector_getLength(pVector) == expectedVec1Size);
	for (i = 0; i < expectedVec1Size; ++i)
		{
		assert((char) vector_elementAt(pVector, i) == expectedVec1[i]);
		}

	assert((char) vector_removeAt(pVector, 0) == '0');
	static const char expectedVec2[] = "12345678";
	static const size_t expectedVec2Size = (sizeof(expectedVec2) / sizeof(char)) - 1;

	assert(vector_getLength(pVector) == expectedVec2Size);
	for (i = 0; i < expectedVec2Size; ++i)
		{
		assert((char) vector_elementAt(pVector, i) == expectedVec2[i]);
		}

	c = (char) vector_removeAt(pVector, 3);
	assert(c == '4');

	static const char expectedVec3[] = "1235678";
	static const size_t expectedVec3Size = (sizeof(expectedVec3) / sizeof(char)) - 1;

	assert(vector_getLength(pVector) == expectedVec3Size);
	for (i = 0; i < expectedVec3Size; ++i)
		{
		assert((char) vector_elementAt(pVector, i) == expectedVec3[i]);
		}

	assert(vector_removeAt(pVector, 5555) == NULL);
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