#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 303
//<Prompt> ["sqlite3_vtab_in_next","sqlite3_stmt_scanstatus_v2","sqlite3_column_double","sqlite3_db_mutex","sqlite3_bind_zeroblob"]
/*<Combination>: [int sqlite3_vtab_in_next(sqlite3_value * pVal, sqlite3_value ** ppOut),
    int sqlite3_stmt_scanstatus_v2(sqlite3_stmt * pStmt, int idx, int iScanStatusOp, int flags, void * pOut),
    double sqlite3_column_double(sqlite3_stmt * , int iCol),
    sqlite3_mutex *sqlite3_db_mutex(sqlite3 * ),
    int sqlite3_bind_zeroblob(sqlite3_stmt * , int , int n)
*/
//<score> 1, nr_unique_branch: 4, p_cov: 0.2
//<Quality> {"diversity":7,"density":7,"covered_api":["sqlite3_column_double"],"uncovered_api":["sqlite3_vtab_in_next","sqlite3_stmt_scanstatus_v2","sqlite3_db_mutex","sqlite3_bind_zeroblob"],"unique_branches":{"sqlite3VdbeExec":[[94547,9,94547,31,1]],"sqlite3AtoF":[[34704,30,34704,37,1],[34705,9,34705,17,0]],"vdbePmaReadBlob":[[14487,20,14487,27,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (!in_file || !out_file) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    char query[256];
    while (fgets(query, sizeof(query), in_file) != NULL) {
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            continue;
        }

        int column_count = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < column_count; i++) {
                double value = sqlite3_column_double(stmt, i);
                fwrite(&value, sizeof(double), 1, out_file);
            }
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}