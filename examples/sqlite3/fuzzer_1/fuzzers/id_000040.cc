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
//<ID> 1101
//<Prompt> ["sqlite3_bind_blob64","sqlite3_msize","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_error_offset"]
/*<Combination>: [int sqlite3_bind_blob64(sqlite3_stmt * , int , const void * , sqlite3_uint64 , void (*)(void *) ),
    sqlite3_uint64 sqlite3_msize(void * ),
    sqlite3_value *sqlite3_value_dup(const sqlite3_value * ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_backup_pagecount(sqlite3_backup * p),
    int sqlite3_bind_text16(sqlite3_stmt * , int , const void * , int , void (*)(void *) ),
    int sqlite3_error_offset(sqlite3 * db)
*/
//<score> 1.3333334, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":9,"density":6,"covered_api":["sqlite3_bind_blob64","sqlite3_msize","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_error_offset"],"uncovered_api":[],"unique_branches":{"sqlite3_value_dup":[[89572,7,89572,15,0]]},"library_calls":["sqlite3_bind_blob64","sqlite3_msize","sqlite3_value_dup","sqlite3_prepare_v2","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_error_offset","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<=34) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set the input file name
    const char *input_file = "input_file";
    
    // Step 4: Create a FILE * variable for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Set the output file name
    const char *output_file = "output_file";
    
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    // Step 2: Call sqlite3_bind_blob64 API
    sqlite3_bind_blob64(stmt, fuzz_int32_t_1, NULL, 0, NULL);
    
    // Step 2: Call sqlite3_msize API
    sqlite3_msize(NULL);
    
    // Step 2: Call sqlite3_value_dup API
    sqlite3_value_dup(NULL);
    
    // Step 2: Call sqlite3_prepare_v2 API
    sqlite3_prepare_v2(db, fuzz_str_2, static_cast<int>(fuzz_str_sz_2), &stmt, NULL);
    
    // Step 2: Call sqlite3_backup_pagecount API
    sqlite3_backup_pagecount(NULL);
    
    // Step 2: Call sqlite3_bind_text16 API
    sqlite3_bind_text16(stmt, fuzz_int32_t_3, NULL, fuzz_int32_t_4, NULL);
    
    // Step 2: Call sqlite3_error_offset API
    sqlite3_error_offset(db);
    
    // Step 7: Release resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}