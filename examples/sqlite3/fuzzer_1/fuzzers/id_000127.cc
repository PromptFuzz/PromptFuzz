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
//<ID> 3279
//<Prompt> ["sqlite3_db_readonly","sqlite3_compileoption_get","sqlite3_str_length","sqlite3_system_errno","sqlite3_extended_errcode","sqlite3_total_changes","sqlite3_create_module","sqlite3_blob_reopen","sqlite3_db_filename"]
/*<Combination>: [int sqlite3_db_readonly(sqlite3 * db, const char * zDbName),
    const char *sqlite3_compileoption_get(int N),
    int sqlite3_str_length(sqlite3_str * ),
    int sqlite3_system_errno(sqlite3 * ),
    int sqlite3_extended_errcode(sqlite3 * db),
    int sqlite3_total_changes(sqlite3 * ),
    int sqlite3_create_module(sqlite3 * db, const char * zName, const sqlite3_module * p, void * pClientData),
    int sqlite3_blob_reopen(sqlite3_blob * , sqlite3_int64 ),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 0.84615386, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":13,"density":11,"covered_api":["sqlite3_db_readonly","sqlite3_compileoption_get","sqlite3_str_length","sqlite3_system_errno","sqlite3_extended_errcode","sqlite3_total_changes","sqlite3_create_module","sqlite3_blob_reopen","sqlite3_db_filename"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_db_readonly","sqlite3_compileoption_get","sqlite3_str_new","sqlite3_str_length","sqlite3_system_errno","sqlite3_extended_errcode","sqlite3_total_changes","sqlite3_create_module","sqlite3_blob_reopen","sqlite3_db_filename","sqlite3_str_finish","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_127(const uint8_t* f_data, size_t f_size) {
	if(f_size<=24) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the input file name
    const char *input_file = "input_file";

    // Initialize sqlite3 database
    sqlite3 *db;
    sqlite3_open(input_file, &db);

    // Step 1: Use sqlite3_db_readonly to check if the database is read-only
    int readonly = sqlite3_db_readonly(db, NULL);
    if (readonly) {
        // Handle read-only case
    }

    // Step 1: Use sqlite3_compileoption_get to get the compile option at index 0
    const char *compile_option = sqlite3_compileoption_get(fuzz_int32_t_1);

    // Step 1: Use sqlite3_str_length to get the length of a sqlite3_str object
    sqlite3_str *str = sqlite3_str_new(db);
    int str_length = sqlite3_str_length(str);

    // Step 1: Use sqlite3_system_errno to get the system error code for the database connection
    int system_errno = sqlite3_system_errno(db);

    // Step 1: Use sqlite3_extended_errcode to get the extended error code for the database connection
    int extended_errcode = sqlite3_extended_errcode(db);

    // Step 1: Use sqlite3_total_changes to get the total number of database rows that have been modified, inserted, or deleted since the database connection was opened
    int total_changes = sqlite3_total_changes(db);

    // Step 1: Use sqlite3_create_module to register a new module with the database connection
    int create_module_result = sqlite3_create_module(db, fuzz_str_2, NULL, NULL);

    // Step 1: Use sqlite3_blob_reopen to reopen a BLOB handle
    sqlite3_blob *blob;
    sqlite3_blob_reopen(blob, fuzz_int64_t_3);

    // Step 1: Use sqlite3_db_filename to get the filename of a database attached to the database connection
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");

    // Step 2: Perform operations with the retrieved data (optional)

    // Step 7: Release resources
    sqlite3_str_finish(str);
    sqlite3_close(db);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}