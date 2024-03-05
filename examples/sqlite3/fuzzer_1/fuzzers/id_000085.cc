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
//<ID> 2272
//<Prompt> ["sqlite3_value_subtype","sqlite3_open","sqlite3_status","sqlite3_column_name","sqlite3_vtab_collation"]
/*<Combination>: [unsigned int sqlite3_value_subtype(sqlite3_value * ),
    int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_status(int op, int * pCurrent, int * pHighwater, int resetFlag),
    const char *sqlite3_column_name(sqlite3_stmt * , int N),
    const char *sqlite3_vtab_collation(sqlite3_index_info * , int )
*/
//<score> 1.8666668, nr_unique_branch: 2, p_cov: 0.8
//<Quality> {"diversity":9,"density":7,"covered_api":["sqlite3_open","sqlite3_status","sqlite3_column_name","sqlite3_vtab_collation"],"uncovered_api":["sqlite3_value_subtype"],"unique_branches":{"sqlite3_vtab_collation":[[163439,7,163439,15,0],[163439,19,163439,46,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_status","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_column_name","sqlite3_finalize","sqlite3_vtab_collation","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




  // Create a FILE pointer for reading the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create a SQLite database pointer
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Read the input data as SQLite commands and execute them
  char *line = nullptr;
  size_t len = 0;
  while (getline(&line, &len, in_file) != -1) {
    rc = sqlite3_exec(db, line, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
      break;
    }
  }
  free(line);
  
  // Query the SQLite database using various APIs
  int current = 0;
  int highwater = 0;
  rc = sqlite3_status(fuzz_int32_t_1, &current, &highwater, fuzz_int32_t_2);
  if (rc == SQLITE_OK) {
    printf("Memory Used: %d\n", current);
    printf("Peak Memory Used: %d\n", highwater);
  }

  sqlite3_stmt *stmt;
  rc = sqlite3_prepare_v2(db, "SELECT * FROM sqlite_master", -1, &stmt, nullptr);
  if (rc == SQLITE_OK) {
    int numColumns = sqlite3_column_count(stmt);
    for (int i = 0; i < numColumns; i++) {
      const char *columnName = sqlite3_column_name(stmt, fuzz_int32_t_3);
      printf("Column Name: %s\n", columnName);
    }
    sqlite3_finalize(stmt);
  }

  sqlite3_index_info info;
  memset(&info, 0, sizeof(info));
  const char *collation = sqlite3_vtab_collation(&info, fuzz_int32_t_4);
  if (collation != nullptr) {
    printf("Collation: %s\n", collation);
  }
  
  // Release resources
  sqlite3_close(db);
  assert_file_closed(&in_file);;
  
  assert_file_closed(&in_file);
	return 0;
}