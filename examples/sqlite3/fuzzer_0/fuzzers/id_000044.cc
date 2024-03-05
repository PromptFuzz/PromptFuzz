#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1191
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_bind_pointer","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_filename_database","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_bind_pointer(sqlite3_stmt * , int , void * , const char * , void (*)(void *) ),
    int sqlite3_bind_double(sqlite3_stmt * , int , double ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    const char *sqlite3_filename_database(sqlite3_filename ),
    int sqlite3_close(sqlite3 * )
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":17,"density":17,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_filename_database","sqlite3_close"],"uncovered_api":["sqlite3_bind_pointer","sqlite3_bind_double"],"unique_branches":{"sqlite3_filename_database":[[181196,7,181196,19,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_filename_database","sqlite3_db_filename","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Open the SQLite database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 2: Create a prepared statement
    sqlite3_stmt *stmt;
    const char *tail;
    rc = sqlite3_prepare_v2(db, (const char*)data, size, &stmt, &tail);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 3: Bind parameters
    int paramCount = sqlite3_bind_parameter_count(stmt);
    for (int i = 0; i < paramCount; i++) {
        // Bind a zero blob
        rc = sqlite3_bind_zeroblob64(stmt, i+1, 256);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    // Step 4: Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Do something with the result rows (optional)
    }
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 5: Finalize the statement
    rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 6: Get the filename of the database
    sqlite3_filename databaseFilename = sqlite3_filename_database(sqlite3_db_filename(db, NULL));

    // Step 7: Close the database
    sqlite3_close(db);

    return 0;
}