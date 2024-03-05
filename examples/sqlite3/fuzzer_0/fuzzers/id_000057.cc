#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1457
//<Prompt> ["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_blob_write(sqlite3_blob * , const void * z, int n, int iOffset),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 0, nr_unique_branch: 1, p_cov: 0
//<Quality> {"diversity":0,"density":1,"covered_api":[],"uncovered_api":["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize"],"unique_branches":{"blobReadWrite":[[101215,27,101215,62,0]]},"library_calls":[]}
/**/

// Event: Write data to a blob in a SQLite database
void  id_000057_write_blob_data(FILE* in_file, FILE* out_file) {
    sqlite3* db;
    sqlite3_blob* blob;
    sqlite3_stmt* stmt;
    int rc, file_len;

    // Open SQLite database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Failed to open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Create table
    rc = sqlite3_exec(db, "CREATE TABLE data (blob BLOB);", 0, 0, 0);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Failed to create table: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Prepare insert statement
    rc = sqlite3_prepare_v2(db, "INSERT INTO data VALUES (?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Bind blob parameter
    rc = sqlite3_bind_parameter_count(stmt);
    if (rc != 1) {
        // Error handling
        fprintf(out_file, "Invalid parameter count: %d\n", rc);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    rc = sqlite3_bind_zeroblob64(stmt, 1, file_len);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Failed to bind zeroblob: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Insert blob data
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // Error handling
        fprintf(out_file, "Failed to insert blob data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Get blob handle
    sqlite3_int64 row_id = sqlite3_last_insert_rowid(db);
    rc = sqlite3_blob_open(db, "main", "data", "blob", row_id, 1, &blob);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Failed to open blob: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Read data from input file and write to blob
    unsigned char buffer[1024];
    size_t bytesRead;
    int iOffset = 0;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        rc = sqlite3_blob_write(blob, buffer, bytesRead, iOffset);
        if (rc != SQLITE_OK) {
            // Error handling
            fprintf(out_file, "Failed to write blob data: %s\n", sqlite3_errmsg(db));
            sqlite3_blob_close(blob);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        iOffset += bytesRead;
    }

    // Close blob and finalize statement
    sqlite3_blob_close(blob);
    sqlite3_finalize(stmt);

    // Close database
    sqlite3_close(db);
}

// Fuzzer test function
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

    FILE* in_file = fmemopen((void*)data, size, "rb");
    FILE* out_file = fopen("output_file", "wb");

    // Perform the event
     id_000057_write_blob_data(in_file, out_file);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}