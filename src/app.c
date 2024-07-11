#include "app.h"

typedef struct _app_state {
  logger_t *logger;
  duckdb_connection connection;
} app_state_t;

app_state_t *app_init(duckdb_database db) {
  app_state_t *state = malloc(sizeof(app_state_t));
  if (duckdb_connect(db, &state->connection) == DuckDBError) {
    fprintf(stderr, "Failed to connect to database\n");
  }

  // Run Initialisation queries
  const char *queries[] = {
      // If we're reusing a database, we need to drop and
      // recreate everything.
      "DROP TABLE IF EXISTS logs;",
      "DROP SEQUENCE IF EXISTS LogIds;",
      "DROP TYPE IF EXISTS LogLevel;",
      // Now we can create the types and the table.
      "CREATE TYPE LogLevel AS ENUM ('DEBUG', 'INFO', 'WARNING', 'ERROR', "
      "'FATAL');",

      "CREATE SEQUENCE LogIds START 1;",

      "CREATE TABLE logs ("
      "id INTEGER PRIMARY KEY DEFAULT nextval('LogIds'),"
      "time TIMESTAMP DEFAULT current_timestamp,"
      "type LogLevel,"
      "source VARCHAR CHECK (len(source) < 64),"
      "content VARCHAR CHECK (len(content) < 256),"
      ");",
  };
  size_t query_count = sizeof(queries) / sizeof(queries[0]);

  duckdb_result result;
  duckdb_state query_state;

  for (size_t query_idx = 0; query_idx < query_count; query_idx++) {
    query_state = duckdb_query(state->connection, queries[query_idx], &result);
    if (query_state == DuckDBError) {
      fprintf(stderr, "Query failed\n");
      exit(1);
    }
    duckdb_destroy_result(&result);
  }

  logger_config_t logger_config = {
      .log_level = INFO,
      .connection = state->connection,
      .mirror_output = true,
  };
  state->logger = logger_new(logger_config);
  return state;
}

void cleanup(app_state_t *state) {
  logger_free(state->logger);
  duckdb_disconnect(&state->connection);
  free(state);
}

void app_log(app_state_t *state, log_level_t level, const char *source,
             const char *content) {
  logger_log(state->logger, level, source, content);
}