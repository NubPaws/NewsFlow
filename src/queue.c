#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "queue.h"
#include "types.h"

/* QueueItem structure definition */
struct QueueItem {
    int prodId;
    NewsType type;
    int newsCount;
    QueueItem *next;
};

/**
 * @brief Creates a new QueueItem.
 */
static QueueItem* qiCreate(int prodId, NewsType type, int newsCount) {
    QueueItem *qi = (QueueItem*)malloc(sizeof(QueueItem));
    if (qi == NULL)
        return NULL;
    qi->prodId = prodId;
    qi->type = type;
    qi->newsCount = newsCount;
    qi->next = NULL;
    return qi;
}

void qiDelete(QueueItem *qi) {
    if (qi == NULL)
        return;
    qi->next = NULL;
    free(qi);
}

void qiDisplay(QueueItem *qi) {
    const char *newsName;
    switch (qi->type) {
        case NEWS:
            newsName = "NEWS";
            break;
        case SPORTS:
            newsName = "SPORTS";
            break;
        case WEATHER:
            newsName = "WEATHER";
            break;
        default:
            newsName = "UNKNOWN";
    }
    printf("Producer %d %s %d\n", qi->prodId, newsName, qi->newsCount);
}

NewsType qiGetType(QueueItem *qi) {
    return qi->type;
}

bool qiIsValid(QueueItem *qi) {
    return qi->prodId != -1;
}

/* Unbounded Queue structure definition */
struct Queue {
    QueueItem *front;
    QueueItem *back;
    size_t size;
    sem_t usageLock;
    sem_t emptyLock;
};

Queue* qCreate() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    if (q == NULL)
        return NULL;
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    sem_init(&q->usageLock, 0, 1);
    sem_init(&q->emptyLock, 0, 0);
    return q;
}

QueueItem* qPop(Queue *q) {
    sem_wait(&q->emptyLock);
    sem_wait(&q->usageLock);
    QueueItem *qi = q->front;
    q->front = q->front->next;
    q->size -= 1;
    qi->next = NULL;
    if (q->size == 0)
        q->back = NULL;
    sem_post(&q->usageLock);
    return qi;
}

bool qPush(Queue *q, int prodId, NewsType type, int newsCount) {
    sem_wait(&q->usageLock);
    QueueItem *qi = qiCreate(prodId, type, newsCount);
    if (qi == NULL) {
        sem_post(&q->usageLock);
        return false;
    }
    if (q->size == 0) {
        q->front = qi;
        q->back = qi;
    } else {
        q->back->next = qi;
        q->back = qi;
    }
    q->size += 1;
    sem_post(&q->emptyLock);
    sem_post(&q->usageLock);
    return true;
}

void qPushItem(Queue *q, QueueItem *qi) {
    sem_wait(&q->usageLock);
    if (q->size == 0) {
        q->front = qi;
        q->back = qi;
    } else {
        q->back->next = qi;
        q->back = qi;
    }
    q->size += 1;
    sem_post(&q->emptyLock);
    sem_post(&q->usageLock);
}

size_t qSize(Queue *q) {
    return q->size;
}

bool qIsEmpty(Queue *q) {
    return q->size == 0;
}

void qDelete(Queue *q) {
    while (!qIsEmpty(q)) {
        qiDelete(qPop(q));
    }
    free(q);
}

/* Bounded Queue structure definition */
struct BoundedQueue {
    QueueItem **items;
    size_t size;
    size_t capacity;
    size_t front;
    size_t back;
    sem_t usageLock;
    sem_t emptyLock;
    sem_t fullLock;
};

BoundedQueue* bqCreate(int bound) {
    BoundedQueue *bq = (BoundedQueue*)malloc(sizeof(BoundedQueue));
    if (bq == NULL)
        return NULL;
    bq->items = (QueueItem**)malloc(sizeof(QueueItem*) * bound);
    if (bq->items == NULL) {
        free(bq);
        return NULL;
    }
    bq->size = 0;
    bq->capacity = bound;
    bq->front = 0;
    bq->back = 0;
    sem_init(&bq->usageLock, 0, 1);
    sem_init(&bq->emptyLock, 0, 0);
    sem_init(&bq->fullLock, 0, bound);
    return bq;
}

QueueItem* bqPop(BoundedQueue *bq) {
    sem_wait(&bq->emptyLock);
    sem_wait(&bq->usageLock);
    QueueItem *qi = bq->items[bq->front];
    bq->items[bq->front] = NULL;
    bq->front = (bq->front + 1) % bq->capacity;
    bq->size -= 1;
    sem_post(&bq->fullLock);
    sem_post(&bq->usageLock);
    return qi;
}

bool bqPush(BoundedQueue *bq, int prodId, NewsType type, int newsCount) {
    sem_wait(&bq->fullLock);
    sem_wait(&bq->usageLock);
    QueueItem *qi = qiCreate(prodId, type, newsCount);
    if (qi == NULL)
        return false;
    bq->items[bq->back] = qi;
    bq->back = (bq->back + 1) % bq->capacity;
    bq->size += 1;
    sem_post(&bq->emptyLock);
    sem_post(&bq->usageLock);
    return true;
}

void bqPushItem(BoundedQueue *bq, QueueItem *qi) {
    sem_wait(&bq->fullLock);
    sem_wait(&bq->usageLock);
    bq->items[bq->back] = qi;
    bq->back = (bq->back + 1) % bq->capacity;
    bq->size += 1;
    sem_post(&bq->emptyLock);
    sem_post(&bq->usageLock);
}

size_t bqSize(BoundedQueue *bq) {
    return bq->size;
}

size_t bqCapacity(BoundedQueue *bq) {
    return bq->capacity;
}

bool bqIsEmpty(BoundedQueue *bq) {
    return bq->size == 0;
}

void bqDelete(BoundedQueue *bq) {
    for (size_t i = 0; i < bq->capacity; i++) {
        qiDelete(bq->items[i]);
    }
    free(bq->items);
    free(bq);
}
