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
//<ID> 3406
//<Prompt> ["sqlite3_result_blob64","sqlite3_vtab_nochange","sqlite3_bind_zeroblob","sqlite3_open16","sqlite3_compileoption_get","sqlite3_uri_parameter","sqlite3_create_module","sqlite3_db_filename"]
/*<Combination>: [void sqlite3_result_blob64(sqlite3_context * , const void * , sqlite3_uint64 , void (*)(void *) ),
    int sqlite3_vtab_nochange(sqlite3_context * ),
    int sqlite3_bind_zeroblob(sqlite3_stmt * , int , int n),
    int sqlite3_open16(const void * filename, sqlite3 ** ppDb),
    const char *sqlite3_compileoption_get(int N),
    const char *sqlite3_uri_parameter(sqlite3_filename z, const char * zParam),
    int sqlite3_create_module(sqlite3 * db, const char * zName, const sqlite3_module * p, void * pClientData),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 0.25, nr_unique_branch: 1, p_cov: 0.125
//<Quality> {"diversity":19,"density":19,"covered_api":["sqlite3_open16"],"uncovered_api":["sqlite3_result_blob64","sqlite3_vtab_nochange","sqlite3_bind_zeroblob","sqlite3_compileoption_get","sqlite3_uri_parameter","sqlite3_create_module","sqlite3_db_filename"],"unique_branches":{"pcache1TruncateUnsafe":[[13917,23,13917,24,1]]},"library_calls":["sqlite3_open16","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_close"]}
/**/


// Event: Read input data and write it to output_file as a blob
void  id_000133_readWriteBlob(const uint8_t *data, size_t size) {
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        return;
    }

    fwrite(data, sizeof(uint8_t), size, out_file);

    assert_file_closed(&out_file);;
}

extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t* f_data, size_t f_size) {
	if(f_size<=69) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Open a new SQLite database
    sqlite3 *db;
    int rc = sqlite3_open16(L"test.db", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a test table
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB)";
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare the INSERT statement
    const char *insertSQL = "INSERT INTO test (data) VALUES (?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind the input data as a blob
    rc = sqlite3_bind_blob64(stmt, fuzz_int32_t_2, data, size, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Execute the INSERT statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    // Retrieve the inserted data as a blob
    const char *selectSQL = "SELECT data FROM test";
    rc = sqlite3_prepare_v2(db, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Read the blob data from the result
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const void *blobData = sqlite3_column_blob(stmt, 0);
        int blobSize = sqlite3_column_bytes(stmt, fuzz_int32_t_4);
         id_000133_readWriteBlob((const uint8_t *)blobData, blobSize);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}