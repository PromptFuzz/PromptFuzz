#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3322
//<Prompt> ["sqlite3_db_readonly","sqlite3_result_blob64","sqlite3_compileoption_get","sqlite3_str_length","sqlite3_system_errno","sqlite3_extended_errcode","sqlite3_total_changes","sqlite3_database_file_object","sqlite3_create_module","sqlite3_db_filename"]
/*<Combination>: [int sqlite3_db_readonly(sqlite3 * db, const char * zDbName),
    void sqlite3_result_blob64(sqlite3_context * , const void * , sqlite3_uint64 , void (*)(void *) ),
    const char *sqlite3_compileoption_get(int N),
    int sqlite3_str_length(sqlite3_str * ),
    int sqlite3_system_errno(sqlite3 * ),
    int sqlite3_extended_errcode(sqlite3 * db),
    int sqlite3_total_changes(sqlite3 * ),
    sqlite3_file *sqlite3_database_file_object(const char * ),
    int sqlite3_create_module(sqlite3 * db, const char * zName, const sqlite3_module * p, void * pClientData),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 0.15, nr_unique_branch: 0, p_cov: 0.2
//<Quality> {"diversity":4,"density":3,"covered_api":["sqlite3_db_readonly","sqlite3_compileoption_get"],"uncovered_api":["sqlite3_result_blob64","sqlite3_str_length","sqlite3_system_errno","sqlite3_extended_errcode","sqlite3_total_changes","sqlite3_database_file_object","sqlite3_create_module","sqlite3_db_filename"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_db_readonly","sqlite3_compileoption_get","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t* data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Get the input file name
    const char* input_file = "input_file";
    
    // Step 2: Call the sqlite3 APIs
    sqlite3* db;
    int rc = sqlite3_open(input_file, &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Call the sqlite3_db_readonly API
    int readonly = sqlite3_db_readonly(db, input_file);
    
    // Step 3: Call the sqlite3_compileoption_get API
    const char* compile_option = sqlite3_compileoption_get(0);

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 7: Release resources
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}