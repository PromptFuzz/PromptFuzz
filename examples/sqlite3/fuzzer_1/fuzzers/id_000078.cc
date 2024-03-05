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
//<ID> 2011
//<Prompt> ["sqlite3_blob_open","sqlite3_drop_modules","sqlite3_value_bytes16","sqlite3_aggregate_context","sqlite3_str_value"]
/*<Combination>: [int sqlite3_blob_open(sqlite3 * , const char * zDb, const char * zTable, const char * zColumn, sqlite3_int64 iRow, int flags, sqlite3_blob ** ppBlob),
    int sqlite3_drop_modules(sqlite3 * db, const char ** azKeep),
    int sqlite3_value_bytes16(sqlite3_value * ),
    void *sqlite3_aggregate_context(sqlite3_context * , int nBytes),
    char *sqlite3_str_value(sqlite3_str * )
*/
//<score> 0.6, nr_unique_branch: 2, p_cov: 0.2
//<Quality> {"diversity":20,"density":20,"covered_api":["sqlite3_blob_open"],"uncovered_api":["sqlite3_drop_modules","sqlite3_value_bytes16","sqlite3_aggregate_context","sqlite3_str_value"],"unique_branches":{"sqlite3_blob_bytes":[[101292,11,101292,12,0],[101292,16,101292,24,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* f_data, size_t f_size) {
	if(f_size<=94) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // Create a SQLite database in memory
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table in the database
    const char *createTableSQL = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(db, createTableSQL, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert data into the table
    const char *insertDataSQL = "INSERT INTO test (id, name) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind parameters and execute the statement
    int id = 1;
    const char *name = "";

    sqlite3_bind_int(stmt, 1, fuzz_int32_t_2);
    sqlite3_bind_text(stmt, fuzz_int32_t_4, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return 0;
    }

    // Read data from the table
    const char *selectDataSQL = "SELECT * FROM test;";
    rc = sqlite3_exec(db, fuzz_str_5, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a blob object
    sqlite3_blob *blob;
    rc = sqlite3_blob_open(db, "main", "test", "name", fuzz_int64_t_6, fuzz_int32_t_7, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Read the blob data
    char *blobData = (char *)malloc(1024);
    int blobSize = sqlite3_blob_bytes(blob);
    rc = sqlite3_blob_read(blob, blobData, blobSize, fuzz_int32_t_8);
    if (rc != SQLITE_OK) {
        sqlite3_blob_close(blob);
        free(blobData);
        sqlite3_close(db);
        return 0;
    }

    // Output the blob data to a file
    FILE *outputFile = fopen("output.txt", "wb");
    if (outputFile) {
        fwrite(blobData, 1, blobSize, outputFile);
        assert_file_closed(&outputFile);;
    }

    // Free resources
    sqlite3_blob_close(blob);
    free(blobData);
    sqlite3_close(db);

    assert_file_closed(&outputFile);
	return 0;
}