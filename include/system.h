#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "queue.h"

/**
 * @brief Shared system context for the multithreaded news processing.
 */
typedef struct {
    BoundedQueue **producer_queues; /**< Array of bounded queues, one per producer. */
    int *producer_counts;           /**< Array of article counts per producer. */
    int num_producers;              /**< Number of producers. */
    Queue *news_queues[3];          /**< Array of unbounded queues for NEWS, SPORTS, WEATHER. */
    BoundedQueue *screen_queue;     /**< Bounded queue for screen display. */
} SystemContext;

#endif // _SYSTEM_H
