#ifndef _SAFE_ALLOC_H
#define _SAFE_ALLOC_H

#include <stdlib.h>

/**
 * @brief Allocates memory safely.
 *
 * Exits the program if allocation fails.
 *
 * @param size The size of memory to allocate.
 * @return Pointer to the allocated memory.
 */
void* safeMalloc(size_t size);

/**
 * @brief Reallocates memory safely.
 *
 * Exits the program if allocation fails.
 *
 * @param ptr Pointer to previously allocated memory.
 * @param size New size.
 * @return Pointer to the reallocated memory.
 */
void* safeRealloc(void *ptr, size_t size);

#endif // _SAFE_ALLOC_H
