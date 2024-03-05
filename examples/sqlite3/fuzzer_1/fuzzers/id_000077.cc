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
//<ID> 2006
//<Prompt> ["sqlite3_collation_needed16","sqlite3_snapshot_cmp","sqlite3_win32_set_directory16","sqlite3_thread_cleanup","sqlite3_last_insert_rowid"]
/*<Combination>: [int sqlite3_collation_needed16(sqlite3 * , void * , void (*)(void *, sqlite3 *, int, const void *) ),
    int sqlite3_snapshot_cmp(sqlite3_snapshot * p1, sqlite3_snapshot * p2),
    int sqlite3_win32_set_directory16(unsigned long type, const void * zValue),
    void sqlite3_thread_cleanup(),
    sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * )
*/
//<score> 0, nr_unique_branch: 8, p_cov: 0
//<Quality> {"diversity":5,"density":4,"covered_api":[],"uncovered_api":["sqlite3_collation_needed16","sqlite3_snapshot_cmp","sqlite3_win32_set_directory16","sqlite3_thread_cleanup","sqlite3_last_insert_rowid"],"unique_branches":{"unixRemapfile":[[42887,7,42887,12,0],[42897,9,42897,22,1],[42917,9,42917,25,1],[42917,29,42917,36,1],[42923,7,42923,14,1]],"getPageMMap":[[61864,40,61864,68,0],[61894,11,61894,38,0],[61897,11,61897,17,1]]},"library_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_exec","sqlite3_free","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Open a file for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the database file name
    const char *db_file = "database.db";

    // Step 1: Initialize the sqlite3 library
    sqlite3_initialize();

    // Step 2: Open the database
    sqlite3 *db;
    int rc = sqlite3_open_v2(db_file, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 3: Execute SQL statements using the input data
    char *sql = new char[size + 1];
    fread(sql, size, 1, in_file);
    sql[size] = '\0';

    char *err_msg;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);

    // Step 7: Clean up and release resources
    delete[] sql;
    sqlite3_free(err_msg);
    sqlite3_close(db);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}