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
//<ID> 1581
//<Prompt> ["sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_aggregate_count","sqlite3_filename_wal","sqlite3_step"]
/*<Combination>: [int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_aggregate_count(sqlite3_context * ),
    const char *sqlite3_filename_wal(sqlite3_filename ),
    int sqlite3_step(sqlite3_stmt * )
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":12,"density":12,"covered_api":["sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step"],"uncovered_api":["sqlite3_aggregate_count","sqlite3_filename_wal"],"unique_branches":{"syncJournal":[[57083,21,57083,41,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_step","sqlite3_data_count","sqlite3_column_type","sqlite3_column_int64","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<=20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




  // Create a FILE pointer for reading input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a FILE pointer for writing output data
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Create a SQLite database connection
  sqlite3 *db;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Read SQLite statements from input file
  char *sql = nullptr;
  size_t sql_size = 0;
  ssize_t read;
  while ((read = getline(&sql, &sql_size, in_file)) != -1) {
    // Remove newline character
    sql[strcspn(sql, "\n")] = '\0';

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    const char *tail;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, &tail) != SQLITE_OK) {
      continue;
    }

    // Bind parameters if any
    int param_count = sqlite3_bind_parameter_count(stmt);
    for (int i = 1; i <= param_count; i++) {
      sqlite3_bind_int(stmt, i, fuzz_int32_t_1);
    }

    // Execute the SQL statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // Get column count
      int column_count = sqlite3_data_count(stmt);

      // Process each column value
      for (int i = 0; i < column_count; i++) {
        int column_type = sqlite3_column_type(stmt, fuzz_int32_t_2);
        if (column_type == SQLITE_INTEGER) {
          int64_t value = sqlite3_column_int64(stmt, fuzz_int32_t_3);
          fprintf(out_file, "%lld\n", value);
        } else if (column_type == SQLITE_FLOAT) {
          double value = sqlite3_column_double(stmt, fuzz_int32_t_4);
          fprintf(out_file, "%f\n", value);
        } else if (column_type == SQLITE_TEXT) {
          const unsigned char *value = sqlite3_column_text(stmt, fuzz_int32_t_5);
          fprintf(out_file, "%s\n", value);
        }
      }
    }

    // Finalize the SQL statement
    sqlite3_finalize(stmt);
  }

  // Release allocated resources
  free(sql);
  sqlite3_close(db);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}