#ifndef _QUEUE_H
#define _QUEUE_H

#include "types.h"

/**
 * @file queue.h
 * @brief Thread–safe queue and bounded queue interfaces.
 *
 * These data structures and functions provide a thread–safe way to handle
 * FIFO queues used in the news processing system.
 */

/* Forward declaration for QueueItem */
typedef struct QueueItem QueueItem;

/* QueueItem interface */
void qiDelete(QueueItem *qi);
void qiDisplay(QueueItem *qi);
NewsType qiGetType(QueueItem *qi);
bool qiIsValid(QueueItem *qi);

/* Unbounded Queue interface */
typedef struct Queue Queue;

Queue* qCreate();
QueueItem* qPop(Queue *q);
bool qPush(Queue *q, int prodId, NewsType type, int newsCount);
void qPushItem(Queue *q, QueueItem *qi);
size_t qSize(Queue *q);
bool qIsEmpty(Queue *q);
void qDelete(Queue *q);

/* Bounded Queue interface */
typedef struct BoundedQueue BoundedQueue;

BoundedQueue* bqCreate(int bound);
QueueItem* bqPop(BoundedQueue *bq);
bool bqPush(BoundedQueue *bq, int prodId, NewsType type, int newsCount);
void bqPushItem(BoundedQueue *bq, QueueItem *qi);
size_t bqSize(BoundedQueue *bq);
size_t bqCapacity(BoundedQueue *bq);
bool bqIsEmpty(BoundedQueue *bq);
void bqDelete(BoundedQueue *bq);

#endif // _QUEUE_H
