#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 353
//<Prompt> ["sqlite3_prepare16","sqlite3_snapshot_get","sqlite3_error_offset","sqlite3_value_text16","sqlite3_column_database_name16","sqlite3_mutex_enter"]
/*<Combination>: [int sqlite3_prepare16(sqlite3 * db, const void * zSql, int nByte, sqlite3_stmt ** ppStmt, const void ** pzTail),
    int sqlite3_snapshot_get(sqlite3 * db, const char * zSchema, sqlite3_snapshot ** ppSnapshot),
    int sqlite3_error_offset(sqlite3 * db),
    const void *sqlite3_value_text16(sqlite3_value * ),
    const void *sqlite3_column_database_name16(sqlite3_stmt * , int ),
    void sqlite3_mutex_enter(sqlite3_mutex * )
*/
//<score> 1, nr_unique_branch: 2, p_cov: 0.33333334
//<Quality> {"diversity":8,"density":8,"covered_api":["sqlite3_snapshot_get","sqlite3_error_offset"],"uncovered_api":["sqlite3_prepare16","sqlite3_value_text16","sqlite3_column_database_name16","sqlite3_mutex_enter"],"unique_branches":{"sqlite3_snapshot_get":[[181291,7,181291,32,1],[181297,7,181297,24,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v3","sqlite3_snapshot_get","sqlite3_error_offset","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file as FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Use input_file string directly as file name
    const char *input_file = "input_file";

    // Step 1: Prepare the statement
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *tail;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    rc = sqlite3_prepare_v3(db, "CREATE TABLE test (id INT, name TEXT);", -1, 0, &stmt, &tail);
    if (rc == SQLITE_OK) {
        // Step 2: Get snapshot
        sqlite3_snapshot *snapshot;
        rc = sqlite3_snapshot_get(db, "main", &snapshot);

        // Step 3: Get error offset
        int error_offset = sqlite3_error_offset(db);

        // Step 1 (cont.): Execute the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            // Step 1 (cont.): Finalize the statement
            rc = sqlite3_finalize(stmt);
            if (rc != SQLITE_OK) {
                assert_file_closed(&in_file);;
                sqlite3_close(db);
                assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
            }
        }
    }

    // Step 7: Close input file and release resources
    assert_file_closed(&in_file);;
    sqlite3_close(db);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}