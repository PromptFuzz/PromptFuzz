#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1583
//<Prompt> ["sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_aggregate_count","sqlite3_filename_wal","sqlite3_step"]
/*<Combination>: [int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_aggregate_count(sqlite3_context * ),
    const char *sqlite3_filename_wal(sqlite3_filename ),
    int sqlite3_step(sqlite3_stmt * )
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":10,"density":10,"covered_api":["sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step"],"uncovered_api":["sqlite3_aggregate_count","sqlite3_filename_wal"],"unique_branches":{"pager_end_transaction":[[58329,30,58329,46,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_text","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Open the input data as a file for reading
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a new in-memory database
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Read the SQLite statements from the input file
  char *sql = nullptr;
  size_t sql_len = 0;
  ssize_t bytes_read;
  while ((bytes_read = getline(&sql, &sql_len, in_file)) != -1) {
    // Trim the newline character at the end of the line
    sql[bytes_read - 1] = '\0';

    // Prepare the SQLite statement
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
      continue;
    }

    // Bind parameters if the statement has any
    int param_count = sqlite3_bind_parameter_count(stmt);
    for (int i = 1; i <= param_count; i++) {
      rc = sqlite3_bind_int(stmt, i, fuzz_int32_t_1);
      if (rc != SQLITE_OK) {
        continue;
      }
    }

    // Execute the statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // Fetch the result of the statement
      int column_count = sqlite3_column_count(stmt);
      for (int i = 0; i < column_count; i++) {
        if (sqlite3_column_type(stmt, fuzz_int32_t_2) == SQLITE_TEXT) {
          const char *text = (const char *)sqlite3_column_text(stmt, fuzz_int32_t_3);
          if (text != nullptr) {
            // Write the result to the output file
            FILE *out_file = fopen("output_file", "a");
            if (out_file != nullptr) {
              fprintf(out_file, "%s\n", text);
              assert_file_closed(&out_file);;
            }
          }
        }
      }
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
  }

  // Close the input file
  assert_file_closed(&in_file);;

  // Release the memory for the SQL statement
  free(sql);

  // Close the database connection
  sqlite3_close(db);

  assert_file_closed(&in_file);
	return 0;
}