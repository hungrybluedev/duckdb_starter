#include <duckdb.h>
#include <stdio.h>

#define DB_PATH "db/data.db"

int main() {
  duckdb_database db;
  duckdb_connection con;
  duckdb_state state;
  duckdb_result result;

  if (duckdb_open(DB_PATH, &db) == DuckDBError) {
    fprintf(stderr, "Failed to open database\n");
  }
  if (duckdb_connect(db, &con) == DuckDBError) {
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
      "CREATE TYPE LogLevel AS ENUM ('DEBUG', 'INFO', 'WARN', 'ERROR', "
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

  for (size_t query_idx = 0; query_idx < query_count; query_idx++) {
    state = duckdb_query(con, queries[query_idx], &result);
    printf("Executing query %s\n", queries[query_idx]);
    if (state == DuckDBError) {
      fprintf(stderr, "Query failed\n");
      return 1;
    }
    duckdb_destroy_result(&result);
  }

  // Insert a log entry
  const char *insert_query =
      "INSERT INTO logs (type, source, content) VALUES ($1, $2, $3)";

  duckdb_prepared_statement stmt;
  if (duckdb_prepare(con, insert_query, &stmt) == DuckDBError) {
    fprintf(stderr, "Failed to prepare statement\n");
    return 1;
  }

  duckdb_bind_varchar(stmt, 1, "INFO");
  duckdb_bind_varchar(stmt, 2, "main.c");
  duckdb_bind_varchar(stmt, 3, "Hello, World!");

  duckdb_execute_prepared(stmt, NULL);
  duckdb_destroy_prepare(&stmt);

  // // print the above result to CSV format using `duckdb_value_varchar`
  // idx_t row_count = duckdb_row_count(&result);
  // idx_t column_count = duckdb_column_count(&result);
  // for (idx_t row = 0; row < row_count; row++) {
  //   for (idx_t col = 0; col < column_count; col++) {
  //     if (col > 0) printf(",");
  //     char* str_val = duckdb_value_varchar(&result, col, row);
  //     printf("%s", str_val);
  //     duckdb_free(str_val);
  //   }
  //   printf("\n");
  // }

  // cleanup
  duckdb_disconnect(&con);
  duckdb_close(&db);
}