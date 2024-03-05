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
//<ID> 2320
//<Prompt> ["sqlite3_extended_errcode","sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_get_autocommit","sqlite3_snprintf"]
/*<Combination>: [int sqlite3_extended_errcode(sqlite3 * db),
    sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    int sqlite3_create_collation16(sqlite3 * , const void * zName, int eTextRep, void * pArg, int (*)(void *, int, const void *, int, const void *) xCompare),
    int sqlite3_get_autocommit(sqlite3 * ),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 3.3613446, nr_unique_branch: 4, p_cov: 0.71428573
//<Quality> {"diversity":17,"density":16,"covered_api":["sqlite3_extended_errcode","sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_get_autocommit"],"uncovered_api":["sqlite3_create_collation16","sqlite3_snprintf"],"unique_branches":{"sqlite3VdbeExec":[[93804,9,93804,13,0],[93807,12,93807,20,1],[93814,11,93814,27,1],[93819,13,93819,17,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_last_insert_rowid","sqlite3_get_autocommit","sqlite3_extended_errcode","sqlite3_keyword_count","sqlite3_create_collation","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<=39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




  sqlite3 *db;
  sqlite3_stmt *stmt;

  // Create a file to read input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Open a database connection
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Read query from input file
  char query[256];
  if (fgets(query, sizeof(query), in_file) == NULL) {
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    assert_file_closed(&in_file);
	return 0;
  }

  // Prepare the query statement
  rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    assert_file_closed(&in_file);
	return 0;
  }

  // Bind blob data to the statement
  const char *blob_data = "Hello, World!";
  rc = sqlite3_bind_blob(stmt, fuzz_int32_t_2, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);
	return 0;
  }

  // Execute the statement
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    assert_file_closed(&in_file);;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);
	return 0;
  }

  // Get the last inserted row id
  sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);

  // Get the auto-commit mode
  int autocommit = sqlite3_get_autocommit(db);

  // Get the extended error code
  int extended_errcode = sqlite3_extended_errcode(db);

  // Get the keyword count
  int keyword_count = sqlite3_keyword_count();

  // Create a collation function
  sqlite3_create_collation(db, fuzz_str_3, fuzz_int32_t_4, NULL, NULL);

  // Print the results
  char output[256];
  snprintf(output, sizeof(output), "Last Row ID: %lld, Autocommit: %d, Extended Error Code: %d, Keyword Count: %d\n",
           last_row_id, autocommit, extended_errcode, keyword_count);
  printf("%s", output);

  // Create a file to write output data
  FILE *out_file = fopen("output_file", "wb");
  if (out_file != NULL) {
    fwrite(output, sizeof(char), strlen(output), out_file);
    assert_file_closed(&out_file);;
  }

  // Release resources
  assert_file_closed(&in_file);;
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}