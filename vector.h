//
// Created by cdeler on 10/20/17.
//

#ifndef CMOCKER_VECTOR_H
#define CMOCKER_VECTOR_H

#include <stdlib.h>

#define ENLARGE_FACTOR(_x) (10ULL + (_x) * 3 / 2)

typedef void (*VectorDeleter)(void *vectorItem);

typedef struct
    {
    #define VECTOR_EYECATCHER_SIZE (4U)
    #define VECTOR_EYECATCHER_VALUE ("EYE")
    char eyeCatcher[VECTOR_EYECATCHER_SIZE];
    size_t vectorSize;
    size_t elementsCount;
    void **items;

    VectorDeleter deleter;
    } Vector;

Vector *vector_open(void);
int vector_close(Vector **ppVector) __attribute__((nonnull (1)));
int vector_add(Vector *pVector, void *item) __attribute__((nonnull (1, 2)));

void vector_set_deleter(Vector *pVector, VectorDeleter deleter) __attribute__((nonnull (1, 2)));
void vector_enlarge(Vector *pVector) __attribute__((nonnull (1)));

#endif //CMOCKER_VECTOR_H
