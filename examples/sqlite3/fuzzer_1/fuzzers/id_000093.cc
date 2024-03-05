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
//<ID> 2358
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    int sqlite3_get_autocommit(sqlite3 * ),
    int sqlite3_config(int ),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 0.9047619, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":21,"density":19,"covered_api":["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_get_autocommit","sqlite3_keyword_count","sqlite3_config","sqlite3_snprintf","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* f_data, size_t f_size) {
	if(f_size<=46) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Create a file to read the input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Open a database connection
  sqlite3* db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a table
  const char* create_table_sql = "CREATE TABLE IF NOT EXISTS Test (id INTEGER PRIMARY KEY, data BLOB);";
  rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Insert data into the table
  const char* insert_sql = "INSERT INTO Test (data) VALUES (?);";
  sqlite3_stmt* stmt;
  rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, nullptr);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Read binary data from the input file and bind it to the prepared statement
  uint8_t buffer[1024];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
    rc = sqlite3_bind_blob(stmt, fuzz_int32_t_2, buffer, bytes_read, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
      sqlite3_finalize(stmt);
      sqlite3_close(db);
      assert_file_closed(&in_file);;
      assert_file_closed(&in_file);
	return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
      sqlite3_finalize(stmt);
      sqlite3_close(db);
      assert_file_closed(&in_file);;
      assert_file_closed(&in_file);
	return 0;
    }

    rc = sqlite3_reset(stmt);
    if (rc != SQLITE_OK) {
      sqlite3_finalize(stmt);
      sqlite3_close(db);
      assert_file_closed(&in_file);;
      assert_file_closed(&in_file);
	return 0;
    }
  }

  sqlite3_finalize(stmt);

  // Get the last inserted row ID
  sqlite3_int64 last_insert_rowid = sqlite3_last_insert_rowid(db);

  // Get the autocommit mode
  int autocommit = sqlite3_get_autocommit(db);

  // Get the number of keywords in SQLite
  int keyword_count = sqlite3_keyword_count();

  // Configure SQLite
  sqlite3_config(fuzz_int32_t_3);

  // Snprintf example
  char buffer2[1024];
  sqlite3_snprintf(sizeof(buffer2), buffer2, "Last inserted row ID: %lld", last_insert_rowid);

  // Release resources
  sqlite3_close(db);
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	return 0;
}