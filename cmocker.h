//
// Created by cdeler on 10/19/17.
//

#pragma once
#ifndef CMOCKER_CMOCKER_H
#define CMOCKER_CMOCKER_H

#define __cmocker_unused __attribute__((unused))

int cmocker_mock(void *originalFunction, void *mockFunction);

#endif //CMOCKER_CMOCKER_H
