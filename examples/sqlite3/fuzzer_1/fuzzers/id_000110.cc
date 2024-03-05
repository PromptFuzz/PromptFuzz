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
//<ID> 2778
//<Prompt> ["sqlite3_extended_result_codes","sqlite3_open","sqlite3_set_last_insert_rowid","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [int sqlite3_extended_result_codes(sqlite3 * , int onoff),
    int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    void sqlite3_set_last_insert_rowid(sqlite3 * , sqlite3_int64 ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 1.7142857, nr_unique_branch: 1, p_cov: 0.85714287
//<Quality> {"diversity":11,"density":11,"covered_api":["sqlite3_extended_result_codes","sqlite3_open","sqlite3_set_last_insert_rowid","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_bind_zeroblob64"],"unique_branches":{"sqlite3_set_last_insert_rowid":[[177606,7,177606,32,1]]},"library_calls":["sqlite3_open","sqlite3_extended_result_codes","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_last_insert_rowid","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *tail;

    // Open SQLite database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Enable extended result codes
    sqlite3_extended_result_codes(db, fuzz_int32_t_1);

    // Prepare SQL statement
    rc = sqlite3_prepare_v2(db, (const char *)data, size, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step through the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Process the result row
    }
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Set the last insert rowid
    sqlite3_set_last_insert_rowid(db, sqlite3_last_insert_rowid(db));

    // Close the database
    sqlite3_close(db);

    return 0;
}