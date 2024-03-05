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
//<ID> 232
//<Prompt> ["sqlite3_blob_close","sqlite3_cancel_auto_extension","sqlite3_win32_set_directory","sqlite3_set_authorizer","sqlite3_str_reset"]
/*<Combination>: [int sqlite3_blob_close(sqlite3_blob * ),
    int sqlite3_cancel_auto_extension(void (*)() xEntryPoint),
    int sqlite3_win32_set_directory(unsigned long type, void * zValue),
    int sqlite3_set_authorizer(sqlite3 * , int (*)(void *, int, const char *, const char *, const char *, const char *) xAuth, void * pUserData),
    void sqlite3_str_reset(sqlite3_str * )
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":21,"density":21,"covered_api":["sqlite3_blob_close"],"uncovered_api":["sqlite3_cancel_auto_extension","sqlite3_win32_set_directory","sqlite3_set_authorizer","sqlite3_str_reset"],"unique_branches":{"btreeParseCellPtr":[[13906,27,13906,28,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<=97) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Step 1: Define the required variables
    sqlite3 *db;
    sqlite3_blob *blob;
    sqlite3_str *str;

    // Step 2: Open a database connection
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Step 3: Create a table and insert data
    rc = sqlite3_exec(db, "CREATE TABLE test (id INT PRIMARY KEY, data BLOB)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_exec(db, fuzz_str_1, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step 4: Prepare a statement to update the data
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v3(db, fuzz_str_2, static_cast<int>(fuzz_str_sz_2), fuzz_uint32_t_3, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 5: Bind the input data to the statement
    rc = sqlite3_bind_blob(stmt, fuzz_int32_t_4, data, size, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 6: Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 7: Finalize the statement
    sqlite3_finalize(stmt);

    // Step 8: Open the blob for reading
    rc = sqlite3_blob_open(db, "main", "test", "data", fuzz_int64_t_5, fuzz_int32_t_6, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 9: Read the blob data
    char *buffer = (char *) malloc(size);
    int bytesRead = 0;
    if (buffer) {
        rc = sqlite3_blob_read(blob, buffer, size, fuzz_int32_t_7);
        if (rc != SQLITE_OK) {
            free(buffer);
            sqlite3_blob_close(blob);
            sqlite3_close(db);
            return 0;
        }
    }

    // Step 10: Write the blob data to an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(buffer, 1, bytesRead, out_file);
        assert_file_closed(&out_file);;
    }

    // Step 11: Close the blob and the database connection
    free(buffer);
    sqlite3_blob_close(blob);
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}