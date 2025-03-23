#ifndef _THREAD_FUNCS_H
#define _THREAD_FUNCS_H

#include "system.h"

/**
 * @brief Starts all producer, dispatcher, co-producer, and screen writer threads.
 *
 * @param ctx Pointer to the shared system context.
 */
void startThreads(SystemContext *ctx);

/**
 * @brief Joins all threads and cleans up thread resources.
 *
 * @param ctx Pointer to the shared system context.
 */
void joinThreads(SystemContext *ctx);

#endif // _THREAD_FUNCS_H
