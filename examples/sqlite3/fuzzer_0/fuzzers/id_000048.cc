#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1259
//<Prompt> ["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_wal_checkpoint_v2","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    int sqlite3_txn_state(sqlite3 * , const char * zSchema),
    int sqlite3_wal_checkpoint_v2(sqlite3 * db, const char * zDb, int eMode, int * pnLog, int * pnCkpt),
    int sqlite3_close(sqlite3 * )
*/
//<score> 2.6666667, nr_unique_branch: 2, p_cov: 0.8888889
//<Quality> {"diversity":8,"density":8,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_close"],"uncovered_api":["sqlite3_txn_state"],"unique_branches":{"sqlite3_wal_checkpoint_v2":[[178985,7,178985,12,1],[178986,7,178986,13,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a virtual file for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open a new SQLite database connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    char *sql = nullptr;
    size_t len = 0;
    ssize_t read;

    // Read the input data from the file into a SQL string
    while ((read = getline(&sql, &len, in_file)) != -1) {
        // Remove the trailing newline character
        if (sql[read - 1] == '\n') {
            sql[read - 1] = '\0';
        }

        // Prepare the SQL statement
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            break;
        }
        
        // Bind any parameters in the SQL statement
        int paramCount = sqlite3_bind_parameter_count(stmt);
        for (int i = 1; i <= paramCount; i++) {
            rc = sqlite3_bind_zeroblob64(stmt, i, 0);
            if (rc != SQLITE_OK) {
                break;
            }
        }

        // Execute the SQL statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Do nothing
        }

        // Finalize the SQL statement
        rc = sqlite3_finalize(stmt);
        if (rc != SQLITE_OK) {
            break;
        }
    }

    // Checkpoint the WAL file
    if (rc == SQLITE_OK) {
        rc = sqlite3_wal_checkpoint_v2(db, nullptr, SQLITE_CHECKPOINT_PASSIVE, nullptr, nullptr);
    }

    // Close the database connection
    sqlite3_close(db);
    assert_file_closed(&in_file);;

    // Free the allocated SQL string
    free(sql);

    assert_file_closed(&in_file);
	return 0;
}