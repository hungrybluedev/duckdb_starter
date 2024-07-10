#ifndef PARITY_LOG_H
#define PARITY_LOG_H

#include <stdbool.h>
#include <stdio.h>

typedef struct _logger_struct logger_t;

typedef enum _log_levels {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL,
} log_level_t;

typedef struct _logger_config {
  bool mirror_output;
  FILE *output_stream;
  log_level_t log_level;
} logger_config_t;

logger_t *logger_new();

void logger_free();

#endif  // PARITY_LOG_H