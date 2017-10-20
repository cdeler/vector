//
// Created by cdeler on 10/20/17.
//

#include <assert.h>

#include "cmocker.h"
#include "vector.h"

void test1()
    {
    Vector *v = vector_open();

    assert(v->deleter == NULL);
    assert(v->vectorSize == 0);
    assert(v->items == 0);

    int rc = vector_close(&v);
    assert(v == NULL);
    assert(rc == 0);
    }
int main(int argc __cmocker_unused, char **argv __cmocker_unused)
    {
    test1();

    return 0;
    }