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
//<ID> 2789
//<Prompt> ["sqlite3_extended_result_codes","sqlite3_set_auxdata","sqlite3_open","sqlite3_set_last_insert_rowid","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [int sqlite3_extended_result_codes(sqlite3 * , int onoff),
    void sqlite3_set_auxdata(sqlite3_context * , int N, void * , void (*)(void *) ),
    int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    void sqlite3_set_last_insert_rowid(sqlite3 * , sqlite3_int64 ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 6.8571434, nr_unique_branch: 11, p_cov: 0.5714286
//<Quality> {"diversity":5,"density":5,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_extended_result_codes","sqlite3_set_auxdata","sqlite3_set_last_insert_rowid"],"unique_branches":{"pager_playback_one_page":[[58643,34,58643,66,0],[58648,8,58648,42,0],[58649,7,58649,15,0],[58649,7,58649,15,1],[58663,9,58663,32,1],[58666,9,58666,24,1],[58695,7,58695,10,1],[13906,27,13906,28,0],[13906,27,13906,28,0],[13906,27,13906,28,1],[13906,27,13906,28,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    char *sql = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&sql, &len, in_file)) != -1) {
        // Prepare the SQL statement
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            continue;
        }

        // Step through the statement
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            // Process the rows
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    // Free the SQL statement
    free(sql);

    // Close the database
    sqlite3_close(db);

    // Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}