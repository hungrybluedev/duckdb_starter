#include <duckdb.h>
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "log.h"

#define DB_PATH "db/data.duckdb"

int main() {
  duckdb_database db;

  if (duckdb_open(DB_PATH, &db) == DuckDBError) {
    fprintf(stderr, "Failed to open database\n");
  }

  app_state_t *state = app_init(db);

  app_log(state, INFO, "main", "Hello, world!");
  app_log(state, DEBUG, "main",
          "This is a debug message. Shouldn't "
          "be visible in the log file.");
  app_log(state, WARNING, "main", "This is a warning.");
  app_log(state, FATAL, "main", "This is an error.");

  cleanup(state);
  duckdb_close(&db);
}