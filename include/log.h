#ifndef PARITY_LOG_H
#define PARITY_LOG_H

#include <duckdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _logger_struct logger_t;

typedef enum _log_levels {
  DEBUG = 1,
  INFO,
  WARNING,
  ERROR,
  FATAL,
} log_level_t;

typedef struct _logger_config {
  bool mirror_output;
  FILE *output_stream;
  log_level_t log_level;
  duckdb_connection connection;
} logger_config_t;

logger_t *logger_new(logger_config_t config);

void logger_free(logger_t *logger);

void logger_log(logger_t *logger, log_level_t level, const char *source,
                const char *content);

#endif  // PARITY_LOG_H