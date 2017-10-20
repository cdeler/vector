//
// Created by cdeler on 10/20/17.
//

#ifndef CMOCKER_VECTOR_H
#define CMOCKER_VECTOR_H

#include <stdlib.h>

typedef void (*VectorDeleter)(void *vectorItem);

typedef struct
    {
    size_t vectorSize;
    void *items;

    VectorDeleter deleter;
    } Vector;

Vector *vector_open(void);
int vector_close(Vector **);

void set_deleter(Vector *pVector, VectorDeleter deleter);

#endif //CMOCKER_VECTOR_H
