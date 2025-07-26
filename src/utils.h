#pragma once

#include <stdio.h>
#include <stdlib.h>

#define assert(predicate, message) \
    if (!(predicate))              \
    {                              \
        fprintf(stderr, message);  \
        exit(EXIT_FAILURE);        \
    }                              \
    \