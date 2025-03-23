#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "thread_funcs.h"
#include "queue.h"
#include "safe_alloc.h"
#include "types.h"
#include "system.h"

/* Structures to pass arguments to thread functions. */
typedef struct {
    SystemContext *ctx;
    int producer_id;
} ProducerArgs;

typedef struct {
    SystemContext *ctx;
    int news_type;
} CoProducerArgs;

/* Thread handles (internal to this module) */
static pthread_t *producer_threads = NULL;
static pthread_t dispatcher_thread;
static pthread_t co_producer_threads[3];
static pthread_t screen_writer_thread;
static int num_producer_threads = 0;

/**
 * @brief Sleeps for the specified number of milliseconds.
 *
 * @param msec Milliseconds to sleep.
 */
static void msleep(long msec) {
    usleep(msec * 1000);
}

/**
 * @brief Producer thread function.
 *
 * Reads its assigned article count and pushes news items into its producer queue.
 *
 * @param arg Pointer to ProducerArgs.
 * @return NULL.
 */
static void* producer_thread(void *arg) {
    ProducerArgs *pargs = (ProducerArgs*)arg;
    int id = pargs->producer_id;
    SystemContext *ctx = pargs->ctx;
    BoundedQueue *queue = ctx->producer_queues[id];
    
    int produced_counts[3] = {0, 0, 0};
    int total_articles = ctx->producer_counts[id];
    
    for (int i = 0; i < total_articles; i++) {
        int news_type = i % 3;
        bqPush(queue, id, news_type, produced_counts[news_type]);
        produced_counts[news_type]++;
    }
    /* Signal termination for this producer. */
    bqPush(queue, -1, NEWS, 0);
    free(pargs);
    return NULL;
}

/**
 * @brief Dispatcher thread function.
 *
 * Performs round–robin polling on producer queues and dispatches valid items
 * to the corresponding news queues.
 *
 * @param arg Pointer to SystemContext.
 * @return NULL.
 */
static void* dispatcher_thread_func(void *arg) {
    SystemContext *ctx = (SystemContext*)arg;
    int num = ctx->num_producers;
    bool *active = safeMalloc(sizeof(bool) * num);
    for (int i = 0; i < num; i++) {
        active[i] = true;
    }
    
    while (true) {
        bool any_active = false;
        for (int i = 0; i < num; i++) {
            if (!active[i])
                continue;
            QueueItem *item = bqPop(ctx->producer_queues[i]);
            if (!qiIsValid(item)) {
                active[i] = false;
                qiDelete(item);
            } else {
                int type = qiGetType(item);
                qPushItem(ctx->news_queues[type], item);
                any_active = true;
            }
        }
        if (!any_active)
            break;
    }
    free(active);
    /* Signal termination to co-producer threads. */
    for (int i = 0; i < 3; i++) {
        qPush(ctx->news_queues[i], -1, NEWS, 0);
    }
    return NULL;
}

/**
 * @brief Co-producer thread function.
 *
 * Reads from its assigned news queue and forwards items (with a small delay)
 * to the screen queue.
 *
 * @param arg Pointer to CoProducerArgs.
 * @return NULL.
 */
static void* co_producer_thread_func(void *arg) {
    CoProducerArgs *cargs = (CoProducerArgs*)arg;
    int type = cargs->news_type;
    SystemContext *ctx = cargs->ctx;
    Queue *news_queue = ctx->news_queues[type];
    
    while (true) {
        QueueItem *item = qPop(news_queue);
        if (!qiIsValid(item)) {
            qiDelete(item);
            break;
        }
        msleep(100);
        bqPushItem(ctx->screen_queue, item);
    }
    /* Signal termination for this co-producer. */
    bqPush(ctx->screen_queue, -1, NEWS, 0);
    free(cargs);
    return NULL;
}

/**
 * @brief Screen writer thread function.
 *
 * Continuously pops items from the screen queue and displays them until
 * termination signals from all co-producers are received.
 *
 * @param arg Pointer to SystemContext.
 * @return NULL.
 */
static void* screen_writer_thread_func(void *arg) {
    SystemContext *ctx = (SystemContext*)arg;
    int termination_count = 0;
    while (true) {
        QueueItem *item = bqPop(ctx->screen_queue);
        if (!qiIsValid(item)) {
            qiDelete(item);
            termination_count++;
            if (termination_count == 3)
                break;
            continue;
        }
        qiDisplay(item);
        qiDelete(item);
    }
    return NULL;
}

/**
 * @brief Starts all threads in the system.
 *
 * @param ctx Pointer to the shared system context.
 */
void startThreads(SystemContext *ctx) {
    num_producer_threads = ctx->num_producers;
    producer_threads = safeMalloc(sizeof(pthread_t) * num_producer_threads);
    
    /* Start producer threads */
    for (int i = 0; i < num_producer_threads; i++) {
        ProducerArgs *args = safeMalloc(sizeof(ProducerArgs));
        args->ctx = ctx;
        args->producer_id = i;
        pthread_create(&producer_threads[i], NULL, producer_thread, args);
    }
    
    /* Start dispatcher thread */
    pthread_create(&dispatcher_thread, NULL, dispatcher_thread_func, ctx);
    
    /* Start co-producer threads */
    for (int i = 0; i < 3; i++) {
        CoProducerArgs *args = safeMalloc(sizeof(CoProducerArgs));
        args->ctx = ctx;
        args->news_type = i;
        pthread_create(&co_producer_threads[i], NULL, co_producer_thread_func, args);
    }
    
    /* Start screen writer thread */
    pthread_create(&screen_writer_thread, NULL, screen_writer_thread_func, ctx);
}

/**
 * @brief Joins all threads and releases thread resources.
 *
 * @param ctx Pointer to the shared system context.
 */
void joinThreads(SystemContext *ctx) {
    for (int i = 0; i < num_producer_threads; i++) {
        pthread_join(producer_threads[i], NULL);
    }
    pthread_join(dispatcher_thread, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(co_producer_threads[i], NULL);
    }
    pthread_join(screen_writer_thread, NULL);
    
    free(producer_threads);
}
