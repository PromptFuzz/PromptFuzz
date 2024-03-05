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
//<ID> 1218
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_bind_value","sqlite3_finalize","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_bind_value(sqlite3_stmt * , int , const sqlite3_value * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    int sqlite3_close(sqlite3 * )
*/
//<score> 15, nr_unique_branch: 14, p_cov: 1
//<Quality> {"diversity":12,"density":12,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_bind_value","sqlite3_finalize","sqlite3_close"],"uncovered_api":[],"unique_branches":{"sqlite3_bind_zeroblob":[[90959,7,90959,20,1]],"sqlite3_bind_value":[[90924,5,90924,24,0],[90924,5,90924,24,1],[90928,5,90928,22,0],[90928,5,90928,22,1],[90931,11,90931,37,0],[90935,5,90935,21,0],[90935,5,90935,21,1],[90936,11,90936,35,0],[90936,11,90936,35,1],[90943,5,90943,21,0],[90943,5,90943,21,1],[90948,5,90948,12,0],[90948,5,90948,12,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_bind_value","sqlite3_column_value","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Create and open a temporary database
  sqlite3 *db;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    return 0;
  }

  // Prepare a SQL statement
  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(db, (const char *)data, size, &stmt, nullptr) !=
      SQLITE_OK) {
    sqlite3_close(db);
    return 0;
  }

  // Bind parameters if any
  int paramCount = sqlite3_bind_parameter_count(stmt);
  for (int i = 0; i < paramCount; ++i) {
    // Bind a zero blob
    if (sqlite3_bind_zeroblob64(stmt, i+1, fuzz_uint64_t_1) != SQLITE_OK) {
      sqlite3_finalize(stmt);
      sqlite3_close(db);
      return 0;
    }
  }

  // Execute the statement and retrieve results if any
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    // Process each row
    // ...

    // Bind a value to a parameter
    sqlite3_bind_value(stmt, fuzz_int32_t_2, sqlite3_column_value(stmt, fuzz_int32_t_3));
  }

  // Finalize the statement
  sqlite3_finalize(stmt);

  // Close the database
  sqlite3_close(db);

  return 0;
}