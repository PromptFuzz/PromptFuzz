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
//<ID> 1264
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_vtab_collation","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    int sqlite3_wal_checkpoint_v2(sqlite3 * db, const char * zDb, int eMode, int * pnLog, int * pnCkpt),
    const char *sqlite3_vtab_collation(sqlite3_index_info * , int ),
    int sqlite3_close(sqlite3 * )
*/
//<score> 24.88889, nr_unique_branch: 27, p_cov: 0.8888889
//<Quality> {"diversity":8,"density":8,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_close"],"uncovered_api":["sqlite3_vtab_collation"],"unique_branches":{"checkTreePage":[[80379,11,80379,26,0]],"sqlite3WalCheckpoint":[[68243,7,68243,20,1],[68270,9,68270,22,1],[68280,9,68280,22,1],[68312,7,68312,21,1],[13906,27,13906,28,0],[13906,27,13906,28,0]],"getOverflowPage":[[74499,28,74499,51,1],[74499,55,74499,65,1]],"sqlite3Checkpoint":[[179074,11,179074,26,0],[179081,28,179081,33,0]],"modifyPagePointer":[[73363,7,73363,30,0],[73365,9,73365,38,1],[73383,13,73383,38,1]],"btreeOverwriteOverflowCell":[[78728,11,78728,43,0],[78739,11,78739,25,0]],"walCheckpoint":[[66268,9,66268,78,1],[66340,9,66340,24,0]],"checkList":[[80152,11,80152,34,0],[80152,38,80152,41,0],[80152,38,80152,41,1]],"relocatePage":[[73467,9,73467,20,0],[73469,11,73469,24,1]],"sqlite3OsSync":[[25499,10,25499,15,1]],"sqlite3Pragma":[[137779,15,137779,28,0],[137781,17,137781,21,0],[137787,17,137787,33,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




  // Create a file to read input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Open a database
  sqlite3 *db;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }

  // Read SQLite commands from input file and execute them
  char sql[256];
  while (fgets(sql, sizeof(sql), in_file)) {
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
      continue;
    }

    // Bind zero blob
    int param_count = sqlite3_bind_parameter_count(stmt);
    for (int i = 1; i <= param_count; ++i) {
      sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_1, fuzz_uint64_t_2);
    }

    // Execute the statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      // Executed successfully, process the result if needed
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
  }

  // Checkpoint the database
  int log, ckpt;
  sqlite3_wal_checkpoint_v2(db, NULL, fuzz_int32_t_3, &log, &ckpt);

  // Close the database
  sqlite3_close(db);

  // Close the input file
  assert_file_closed(&in_file);;

  assert_file_closed(&in_file);
	return 0;
}