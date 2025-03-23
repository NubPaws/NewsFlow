#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "thread_funcs.h"
#include "queue.h"
#include "system.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <configuration_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    /* Initialize the system context from the configuration file. */
    SystemContext *ctx = initSystem(argv[1]);
    
    /* Start all threads. */
    startThreads(ctx);
    joinThreads(ctx);
    
    disposeSystem(ctx);
	ctx = NULL;
    
    return EXIT_SUCCESS;
}
