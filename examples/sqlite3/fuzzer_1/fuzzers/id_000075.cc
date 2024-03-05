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
//<ID> 1832
//<Prompt> ["sqlite3_result_subtype","sqlite3_auto_extension","sqlite3_prepare","sqlite3_aggregate_count","sqlite3_column_text"]
/*<Combination>: [void sqlite3_result_subtype(sqlite3_context * , unsigned int ),
    int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_prepare(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_aggregate_count(sqlite3_context * ),
    const unsigned char *sqlite3_column_text(sqlite3_stmt * , int iCol)
*/
//<score> 0.48000002, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":15,"density":9,"covered_api":["sqlite3_prepare","sqlite3_column_text"],"uncovered_api":["sqlite3_result_subtype","sqlite3_auto_extension","sqlite3_aggregate_count"],"unique_branches":{"sqlite3VdbeReset":[[87308,21,87308,31,0]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_prepare","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // 1. Initialize SQLite
    sqlite3_initialize();
    
    // 2. Open a database connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 3. Prepare a SQL statement
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare(db, (const char*)data, size, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // 4. Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // 5. Access data from the result set
        int columnCount = sqlite3_column_count(stmt);
        for (int i = 0; i < columnCount; i++) {
            const unsigned char *text = sqlite3_column_text(stmt, fuzz_int32_t_1);
            if (text != nullptr) {
                // Do something with the text
            }
        }
    }
    
    // 6. Finalize the statement
    sqlite3_finalize(stmt);
    
    // 7. Close the database connection
    sqlite3_close(db);
    
    // 8. Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}