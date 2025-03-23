#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "safe_alloc.h"
#include "queue.h"

//
// This module reads the configuration file and initializes the shared system context.
// The expected configuration file format is as follows:
//   For each producer:
//       <dummy> <producerArticleCount> <producerQueueCapacity>
//   Followed by a single integer indicating the screen queue size.
//
SystemContext* initSystem(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open configuration file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    SystemContext *ctx = safeMalloc(sizeof(SystemContext));
    ctx->producer_queues = NULL;
    ctx->producer_counts = NULL;
    ctx->num_producers = 0;
    ctx->screen_queue = NULL;
    for (int i = 0; i < 3; i++) {
        ctx->news_queues[i] = NULL;
    }
    
    int dummy, prodCount, producerQueueCapacity;
    // Read producer configurations (three integers per producer)
    while (fscanf(file, "%d %d %d", &dummy, &prodCount, &producerQueueCapacity) == 3) {
        ctx->num_producers++;
        ctx->producer_queues = safeRealloc(ctx->producer_queues, sizeof(BoundedQueue*) * ctx->num_producers);
        ctx->producer_counts = safeRealloc(ctx->producer_counts, sizeof(int) * ctx->num_producers);
        ctx->producer_queues[ctx->num_producers - 1] = bqCreate(producerQueueCapacity);
        ctx->producer_counts[ctx->num_producers - 1] = prodCount;
    }
    
    // The last integer in the file specifies the screen queue size.
    int screenQueueSize;
    if (fscanf(file, "%d", &screenQueueSize) != 1) {
        fprintf(stderr, "Error: Failed to read screen queue size from configuration file\n");
        exit(EXIT_FAILURE);
    }
    ctx->screen_queue = bqCreate(screenQueueSize);
    
    // Initialize the three news queues.
    for (int i = 0; i < 3; i++) {
        ctx->news_queues[i] = qCreate();
    }
    
    fclose(file);
    return ctx;
}

void disposeSystem(SystemContext *ctx) {
    /* Clean up producer queues. */
    for (int i = 0; i < ctx->num_producers; i++) {
        bqDelete(ctx->producer_queues[i]);
    }
    free(ctx->producer_queues);
    free(ctx->producer_counts);
    
    /* Clean up news queues and screen queue. */
    for (int i = 0; i < 3; i++) {
        qDelete(ctx->news_queues[i]);
    }
    bqDelete(ctx->screen_queue);
    free(ctx);
}
