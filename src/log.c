#include "log.h"

#include <string.h>

#define LOGGER_INSERT_QUERY \
  "INSERT INTO logs (type, source, content) VALUES ($1, $2, $3)"
#define LEVEL_STR_LEN 7

typedef struct _logger_struct {
  log_level_t log_level;
  bool mirror_output;
  FILE *output_stream;
  duckdb_connection connection;
} logger_t;

logger_t *logger_new(logger_config_t config) {
  logger_t *logger = malloc(sizeof(logger_t));
  if (logger == NULL) {
    return NULL;
  }

  logger->output_stream = config.output_stream;

  if (config.connection == NULL) {
    fprintf(stderr, "No connection provided\n");
    logger_free(logger);
    return NULL;
  }
  logger->connection = config.connection;

  if (config.log_level == 0) {
    logger->log_level = INFO;
  } else {
    logger->log_level = config.log_level;
  }

  logger->mirror_output = config.mirror_output;

  return logger;
}

void logger_free(logger_t *logger) { free(logger); }

static void __log(FILE *stream, const char *level_str, const char *source,
                  const char *content) {
  fputc('[', stream);
  fputs(level_str, stream);

  size_t length = strlen(level_str);
  for (size_t i = length; i < LEVEL_STR_LEN; i++) {
    fputc(' ', stream);
  }

  fputs("] ", stream);
  fputs(source, stream);
  fputs(": ", stream);
  fputs(content, stream);
  fputc('\n', stream);
}

void logger_log(logger_t *logger, log_level_t level, const char *source,
                const char *content) {
  if (level < logger->log_level) {
    return;
  }

  const char *level_str;
  switch (level) {
    case DEBUG:
      level_str = "DEBUG";
      break;
    case INFO:
      level_str = "INFO";
      break;
    case WARNING:
      level_str = "WARNING";
      break;
    case ERROR:
      level_str = "ERROR";
      break;
    case FATAL:
      level_str = "FATAL";
      break;
    default:
      level_str = "UNKNOWN";
      break;
  }

  if (logger->output_stream != NULL) {
    __log(logger->output_stream, level_str, source, content);
  }

  if (logger->mirror_output) {
    __log(stdout, level_str, source, content);
  }

  duckdb_prepared_statement stmt;
  if (duckdb_prepare(logger->connection, LOGGER_INSERT_QUERY, &stmt) ==
      DuckDBError) {
    fprintf(stderr, "Failed to prepare statement\n");
    exit(1);
  }

  duckdb_bind_varchar(stmt, 1, level_str);
  duckdb_bind_varchar(stmt, 2, source);
  duckdb_bind_varchar(stmt, 3, content);

  if (duckdb_execute_prepared(stmt, NULL) == DuckDBError) {
    fprintf(stderr, "Failed to write log.\n");
    exit(1);
  }
  duckdb_destroy_prepare(&stmt);

  if (level == FATAL) {
    duckdb_disconnect(&logger->connection);
    exit(1);
  }
}