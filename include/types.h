#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <inttypes.h>

#define BUFFER_SIZE 1024

/**
 * @brief Buffer type.
 */
typedef char Buffer[BUFFER_SIZE];

/**
 * @brief Enumeration for news types.
 */
typedef enum { NEWS, SPORTS, WEATHER } NewsType;

#endif // _TYPES_H
