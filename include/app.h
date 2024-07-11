#ifndef PARITY_APP_H
#define PARITY_APP_H

#include <duckdb.h>

#include "log.h"

typedef struct _app_state app_state_t;

app_state_t *app_init(duckdb_database db);

void cleanup(app_state_t *state);

void app_log(app_state_t *state, log_level_t level, const char *source,
             const char *content);

#endif  // PARITY_APP_H