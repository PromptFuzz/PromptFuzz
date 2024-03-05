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
//<ID> 3397
//<Prompt> ["sqlite3_result_blob64","sqlite3_vtab_nochange","sqlite3_open16","sqlite3_compileoption_get","sqlite3_uri_parameter","sqlite3_total_changes","sqlite3_create_module","sqlite3_db_filename"]
/*<Combination>: [void sqlite3_result_blob64(sqlite3_context * , const void * , sqlite3_uint64 , void (*)(void *) ),
    int sqlite3_vtab_nochange(sqlite3_context * ),
    int sqlite3_open16(const void * filename, sqlite3 ** ppDb),
    const char *sqlite3_compileoption_get(int N),
    const char *sqlite3_uri_parameter(sqlite3_filename z, const char * zParam),
    int sqlite3_total_changes(sqlite3 * ),
    int sqlite3_create_module(sqlite3 * db, const char * zName, const sqlite3_module * p, void * pClientData),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 0, nr_unique_branch: 0, p_cov: 0
//<Quality> {"diversity":3,"density":3,"covered_api":[],"uncovered_api":["sqlite3_result_blob64","sqlite3_vtab_nochange","sqlite3_open16","sqlite3_compileoption_get","sqlite3_uri_parameter","sqlite3_total_changes","sqlite3_create_module","sqlite3_db_filename"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Create a file to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
  
    // Create a SQLite database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
  
    // Read the input file and execute SQLite commands
    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
  
    while ((read = getline(&line, &len, in_file)) != -1) {
        // Remove the new line character from the end of the line
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
      
        // Execute the SQLite command
        rc = sqlite3_exec(db, line, nullptr, nullptr, nullptr);
      
        // Write the result to the output file
        if (rc == SQLITE_OK) {
            fputs("OK\n", out_file);
        } else {
            fputs("ERROR\n", out_file);
        }
    }
  
    // Clean up
    if (line != nullptr) {
        free(line);
    }
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
  
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}