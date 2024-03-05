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
//<ID> 1246
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    int sqlite3_txn_state(sqlite3 * , const char * zSchema),
    int sqlite3_close(sqlite3 * )
*/
//<score> 2, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":10,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"],"uncovered_api":[],"unique_branches":{"sqlite3AddInt64":[[35852,17,35852,47,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<=19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Open a database connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Prepare a SQL statement
    sqlite3_stmt *stmt;
    const char *tail;
    rc = sqlite3_prepare_v2(db, (const char *)data, size, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind parameters
    int paramCount = sqlite3_bind_parameter_count(stmt);
    for (int i = 1; i <= paramCount; i++) {
        sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_1, fuzz_uint64_t_2);
    }

    // Step through the statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Do something with result data
        // ...
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Check transaction state
    const char *schema = "main";
    rc = sqlite3_txn_state(db, fuzz_str_3);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Close the database connection
    sqlite3_close(db);

    return 0;
}