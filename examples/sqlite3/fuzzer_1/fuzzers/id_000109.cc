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
//<ID> 2748
//<Prompt> ["sqlite3_result_blob","sqlite3_extended_result_codes","sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_collation_needed16","sqlite3_finalize"]
/*<Combination>: [void sqlite3_result_blob(sqlite3_context * , const void * , int , void (*)(void *) ),
    int sqlite3_extended_result_codes(sqlite3 * , int onoff),
    int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_collation_needed16(sqlite3 * , void * , void (*)(void *, sqlite3 *, int, const void *) ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 0.39285713, nr_unique_branch: 0, p_cov: 0.5
//<Quality> {"diversity":14,"density":11,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_result_blob","sqlite3_extended_result_codes","sqlite3_bind_zeroblob64","sqlite3_collation_needed16"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_errmsg","sqlite3_finalize","sqlite3_close"]}
/*Here is the C++ program that achieves the given event using the sqlite3 library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




  // Open a database connection
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }

  // Prepare a query
  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, (const char *)data, size, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot prepare query: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  }

  // Step through the query
  while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    // Fetch the result data and write to output file
    int num_columns = sqlite3_column_count(stmt);
    for (int i = 0; i < num_columns; i++) {
      int column_type = sqlite3_column_type(stmt, fuzz_int32_t_1);
      if (column_type == SQLITE_BLOB) {
        const void *blob_data = sqlite3_column_blob(stmt, i);
        int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_2);

        FILE *out_file = fopen("output_file", "ab");
        fwrite(blob_data, 1, blob_size, out_file);
        assert_file_closed(&out_file);;
      }
    }
  }

  // Check for errors or end of query
  if (rc != SQLITE_DONE) {
    fprintf(stderr, "Error executing query: %s\n", sqlite3_errmsg(db));
  }

  // Cleanup and close connections
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return 0;
}