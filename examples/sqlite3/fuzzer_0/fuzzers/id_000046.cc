#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1233
//<Prompt> ["sqlite3_open","sqlite3_blob_bytes","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_blob_bytes(sqlite3_blob * ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    int sqlite3_txn_state(sqlite3 * , const char * zSchema),
    int sqlite3_close(sqlite3 * )
*/
//<score> 2.6666667, nr_unique_branch: 2, p_cov: 0.8888889
//<Quality> {"diversity":9,"density":9,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"],"uncovered_api":["sqlite3_blob_bytes"],"unique_branches":{"walTryBeginRead":[[67133,7,67133,9,0],[67134,12,67134,27,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_txn_state","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE * variable for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a sqlite3 database handle variable
    sqlite3 *db;

    // Open the sqlite3 database from input file
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read SQL statements from input file and execute them
    char sql[4096];
    while (fgets(sql, sizeof(sql), in_file)) {
        char *sql_end = strchr(sql, '\n');
        if (sql_end != nullptr) {
            *sql_end = '\0';
        }

        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            continue;
        }

        int parameter_count = sqlite3_bind_parameter_count(stmt);
        for (int i = 1; i <= parameter_count; i++) {
            if (sqlite3_bind_zeroblob64(stmt, i, 0) != SQLITE_OK) {
                continue;
            }
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {}

        sqlite3_finalize(stmt);
    }

    // Get the transaction state of the database
    const char *schema = nullptr;
    if (sqlite3_txn_state(db, schema) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Close the sqlite3 database
    sqlite3_close(db);

    // Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}