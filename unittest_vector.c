//
// Created by cdeler on 10/20/17.
//

#include <assert.h>
#include <memory.h>

#include "cmocker.h"
#include "vector.h"

void test1_smoke()
    {
    Vector *v = vector_open();

    assert(v->deleter == NULL);
    assert(v->vectorSize == 0);
    assert(v->items == 0);
    assert(v->elementsCount == 0);
    assert(!strcmp(v->eyeCatcher, VECTOR_EYECATCHER_VALUE));

    int rc = vector_close(&v);
    assert(v == NULL);
    assert(rc == 0);
    }

void test2_vector_enlarge()
    {
    size_t i;
    Vector *v = vector_open();
    vector_enlarge(v);

    assert(v->vectorSize == ENLARGE_FACTOR(0));
    assert(v->items);

    for (i = 0; i < v->vectorSize; ++i)
        {
        void *element = v->items[i];

        assert(element == NULL);
        }

    size_t oldSize = v->vectorSize;
    vector_enlarge(v);

    assert(v->vectorSize == ENLARGE_FACTOR(oldSize));
    for (i = 0; i < v->vectorSize; ++i)
        {
        void *element = v->items[i];

        assert(element == NULL);
        }

    vector_close(&v);
    }

void test3_vector_add()
    {
    Vector *v = vector_open();
    vector_set_deleter(v, free);

    char *element = strdup("foo");
    vector_add(v, element);

    assert(v->elementsCount == 1ULL);
    assert(v->vectorSize == ENLARGE_FACTOR(0));
    assert(v->items);

    char *firstElement = v->items[0];
    assert(!strcmp(firstElement, "foo"));

    vector_close(&v);
    }

int main(int argc __cmocker_unused, char **argv __cmocker_unused)
    {
    test1_smoke();
    test2_vector_enlarge();
    test3_vector_add();

    return 0;
    }