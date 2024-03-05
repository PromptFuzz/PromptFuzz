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
//<ID> 201
//<Prompt> ["sqlite3_prepare16_v3","sqlite3_value_type","sqlite3_column_name16","sqlite3_profile","sqlite3_value_blob"]
/*<Combination>: [int sqlite3_prepare16_v3(sqlite3 * db, const void * zSql, int nByte, unsigned int prepFlags, sqlite3_stmt ** ppStmt, const void ** pzTail),
    int sqlite3_value_type(sqlite3_value * ),
    const void *sqlite3_column_name16(sqlite3_stmt * , int N),
    void *sqlite3_profile(sqlite3 * , void (*)(void *, const char *, sqlite3_uint64) xProfile, void * ),
    const void *sqlite3_value_blob(sqlite3_value * )
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":10,"density":10,"covered_api":[],"uncovered_api":["sqlite3_prepare16_v3","sqlite3_value_type","sqlite3_column_name16","sqlite3_profile","sqlite3_value_blob"],"unique_branches":{"columnName":[[90603,9,90603,22,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_name","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Create a FILE pointer to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");

  // Create a database connection
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Read the SQL query from the input file
  char *sql = nullptr;
  size_t sql_size = 0;
  ssize_t read;
  while ((read = getline(&sql, &sql_size, in_file)) != -1) {
    // Remove the newline character from the end of the SQL query
    if (sql[read - 1] == '\n') {
      sql[read - 1] = '\0';
    }

    // Prepare the SQL query
    sqlite3_stmt *stmt;
    const char *tail;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK) {
      // Execute the prepared SQL query
      while (sqlite3_step(stmt) == SQLITE_ROW) {
        int columns = sqlite3_column_count(stmt);
        for (int i = 0; i < columns; i++) {
          const char *column_name = sqlite3_column_name(stmt, fuzz_int32_t_1);
          int column_type = sqlite3_column_type(stmt, fuzz_int32_t_2);

          printf("Column Name: %s\n", column_name);
          printf("Column Type: %d\n", column_type);

          // Handle different column types
          if (column_type == SQLITE_BLOB) {
            const void *blob_data = sqlite3_column_blob(stmt, i);
            int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_3);
            
            // Process the blob data, if needed
            // ...
          }

          // Handle other column types, if needed
          // ...
        }
      }
      
      // Finalize the prepared statement
      sqlite3_finalize(stmt);
    }
  }

  // Clean up resources
  sqlite3_close(db);
  assert_file_closed(&in_file);;
  free(sql);

  assert_file_closed(&in_file);
	return 0;
}