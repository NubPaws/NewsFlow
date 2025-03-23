#ifndef _CONFIG_H
#define _CONFIG_H

#include "system.h"

/**
 * @brief Initialize the system context from a configuration file.
 *
 * The configuration file is expected to contain several lines each with three integers:
 * <em>dummy</em> <em>producerArticleCount</em> <em>producerQueueCapacity</em>
 * for each producer. After all producer lines, a single integer specifies the screen queue size.
 *
 * @param filename Path to the configuration file.
 * @return Pointer to an initialized SystemContext.
 */
SystemContext* initSystem(const char *filename);

void disposeSystem(SystemContext *ctx);

#endif // _CONFIG_H
