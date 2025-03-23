#include <stdio.h>
#include <stdlib.h>
#include "safe_alloc.h"

void* safeMalloc(size_t size) {
    void *mem = malloc(size);
    if (!mem) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}

void* safeRealloc(void *ptr, size_t size) {
    void *mem = realloc(ptr, size);
    if (!mem) {
        fprintf(stderr, "Error: realloc failed\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}
