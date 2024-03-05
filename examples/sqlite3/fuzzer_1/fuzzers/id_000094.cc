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
//<ID> 2378
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_get_autocommit","sqlite3_config","sqlite3_autovacuum_pages","sqlite3_soft_heap_limit","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_get_autocommit(sqlite3 * ),
    int sqlite3_config(int ),
    int sqlite3_autovacuum_pages(sqlite3 * db, unsigned int (*)(void *, const char *, unsigned int, unsigned int, unsigned int) , void * , void (*)(void *) ),
    void sqlite3_soft_heap_limit(int N),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 10.799999, nr_unique_branch: 11, p_cov: 1
//<Quality> {"diversity":20,"density":18,"covered_api":["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_get_autocommit","sqlite3_config","sqlite3_autovacuum_pages","sqlite3_soft_heap_limit","sqlite3_snprintf"],"uncovered_api":[],"unique_branches":{"sqlite3_soft_heap_limit64":[[29677,7,29677,9,1],[29681,7,29681,10,1],[29685,7,29685,23,0],[29685,28,29685,44,1],[29685,48,29685,52,1],[29693,7,29693,15,0],[29693,7,29693,15,1],[13687,56,13687,59,0],[13687,56,13687,59,0],[13687,56,13687,59,1]],"sqlite3_soft_heap_limit":[[29697,7,29697,10,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_last_insert_rowid","sqlite3_get_autocommit","sqlite3_config","sqlite3_autovacuum_pages","sqlite3_soft_heap_limit","sqlite3_snprintf","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* f_data, size_t f_size) {
	if(f_size<=42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg;

    // Open a memory database
    if(sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create a table
    const char *createTableQuery = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    if(sqlite3_exec(db, createTableQuery, NULL, NULL, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Prepare an SQL statement
    const char *insertQuery = "INSERT INTO test (data) VALUES (?);";
    if(sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind the input data to the prepared statement
    if(sqlite3_bind_blob(stmt, fuzz_int32_t_2, data, size, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Execute the prepared statement
    if(sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Get the last inserted row ID
    sqlite3_int64 lastRowID = sqlite3_last_insert_rowid(db);

    // Check if auto-commit mode is enabled
    int autoCommit = sqlite3_get_autocommit(db);

    // Configure sqlite3 with SQLITE_CONFIG_MALLOC
    sqlite3_config(SQLITE_CONFIG_MALLOC);

    // Perform an auto-vacuum operation
    sqlite3_autovacuum_pages(db, NULL, NULL, NULL);

    // Set the soft heap limit
    sqlite3_soft_heap_limit(1024 * 1024);

    // Use sqlite3_snprintf to format a string
    char buf[256];
    sqlite3_snprintf(256, buf, "Last inserted row ID: %lld\nAuto-commit mode: %d\n", lastRowID, autoCommit);

    // Write the output to a file
    FILE *out_file = fopen("output_file", "wb");
    if(out_file) {
        fwrite(buf, sizeof(char), strlen(buf), out_file);
        assert_file_closed(&out_file);;
    }

    // Release resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}