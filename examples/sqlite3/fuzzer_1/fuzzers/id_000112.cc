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
//<ID> 2848
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_extended_result_codes"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_extended_result_codes(sqlite3 * , int onoff)
*/
//<score> 1, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":11,"density":11,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_extended_result_codes"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_extended_result_codes","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/*Here is the C++ program that follows the instructions provided:

*/


extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Open a new database connection
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, (const char*)data, size, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind parameters
    int paramCount = sqlite3_bind_parameter_count(stmt);
    for (int i = 0; i < paramCount; i++) {
        if (sqlite3_bind_zeroblob64(stmt, i+1, fuzz_uint64_t_1) != SQLITE_OK) { // Example of using sqlite3_bind_zeroblob64
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    // Enable extended result codes
    sqlite3_extended_result_codes(db, fuzz_int32_t_2); // Example of using sqlite3_extended_result_codes

    // Evaluate the prepared statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database connection
    sqlite3_close(db);

    return 0;
}