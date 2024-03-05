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
//<ID> 734
//<Prompt> ["sqlite3_column_origin_name","sqlite3_free","sqlite3_blob_read","sqlite3_db_config","sqlite3_value_text"]
/*<Combination>: [const char *sqlite3_column_origin_name(sqlite3_stmt * , int ),
    void sqlite3_free(void * ),
    int sqlite3_blob_read(sqlite3_blob * , void * Z, int N, int iOffset),
    int sqlite3_db_config(sqlite3 * , int op),
    const unsigned char *sqlite3_value_text(sqlite3_value * )
*/
//<score> 1.4, nr_unique_branch: 6, p_cov: 0.2
//<Quality> {"diversity":21,"density":21,"covered_api":["sqlite3_blob_read"],"uncovered_api":["sqlite3_column_origin_name","sqlite3_free","sqlite3_db_config","sqlite3_value_text"],"unique_branches":{"sqlite3Atoi64":[[35019,13,35019,27,1]],"blobSeekToRow":[[100896,7,100896,21,1],[100919,7,100919,21,1],[100921,13,100921,21,0],[100924,9,100924,22,0]],"sqlite3Prepare":[[140157,7,140157,44,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_text","sqlite3_column_bytes","sqlite3_blob_open","sqlite3_column_int64","sqlite3_blob_read","sqlite3_blob_close","sqlite3_finalize","sqlite3_close"]}
/*Here is the implementation of the LLVMFuzzerTestOneInput_22 function that achieves the desired event using the sqlite3 APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<=89) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    int rc;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    sqlite3_blob *blob;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Create a table named "mytable" with a single column "mycolumn"
    rc = sqlite3_exec(db, "CREATE TABLE mytable (mycolumn TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare an INSERT statement
    rc = sqlite3_prepare_v3(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), fuzz_uint32_t_2, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind a value from the input data to the parameter in the prepared statement
    sqlite3_bind_text(stmt, 1, (const char*)data, size, SQLITE_STATIC);

    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Finalize the prepared statement
    sqlite3_finalize(stmt);

    // Prepare a SELECT statement
    rc = sqlite3_prepare_v3(db, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), fuzz_uint32_t_4, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Execute the SELECT statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Retrieve the value from the result row
        const unsigned char *text = sqlite3_column_text(stmt, 0);

        // Read the data from the blob object
        int blobSize = sqlite3_column_bytes(stmt, fuzz_int32_t_5);
        void *blobData = malloc(blobSize);
        rc = sqlite3_blob_open(db, "main", "mytable", "mycolumn", sqlite3_column_int64(stmt, fuzz_int32_t_7), fuzz_int32_t_6, &blob);
        if (rc == SQLITE_OK) {
            sqlite3_blob_read(blob, blobData, blobSize, fuzz_int32_t_8);
            sqlite3_blob_close(blob);
        }

        // Output the retrieved values to a file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            fwrite(text, sizeof(unsigned char), strlen((const char *)text), out_file);
            fwrite(blobData, 1, blobSize, out_file);
            assert_file_closed(&out_file);;
        }

        free(blobData);
    }

    // Finalize the SELECT statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}