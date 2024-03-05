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
//<ID> 3127
//<Prompt> ["sqlite3_keyword_name","sqlite3_create_window_function","sqlite3_value_encoding","sqlite3_trace_v2","sqlite3_vfs_register","sqlite3_db_filename"]
/*<Combination>: [int sqlite3_keyword_name(int , const char ** , int * ),
    int sqlite3_create_window_function(sqlite3 * db, const char * zFunctionName, int nArg, int eTextRep, void * pApp, void (*)(sqlite3_context *, int, sqlite3_value **) xStep, void (*)(sqlite3_context *) xFinal, void (*)(sqlite3_context *) xValue, void (*)(sqlite3_context *, int, sqlite3_value **) xInverse, void (*)(void *) xDestroy),
    int sqlite3_value_encoding(sqlite3_value * ),
    int sqlite3_trace_v2(sqlite3 * , unsigned int uMask, int (*)(unsigned int, void *, void *, void *) xCallback, void * pCtx),
    int sqlite3_vfs_register(sqlite3_vfs * , int makeDflt),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 1.1851852, nr_unique_branch: 1, p_cov: 0.6666667
//<Quality> {"diversity":9,"density":8,"covered_api":["sqlite3_create_window_function","sqlite3_trace_v2","sqlite3_vfs_register","sqlite3_db_filename"],"uncovered_api":["sqlite3_keyword_name","sqlite3_value_encoding"],"unique_branches":{"sqlite3_vfs_register":[[25813,7,25813,14,0]]},"library_calls":["sqlite3_open","sqlite3_create_window_function","sqlite3_trace_v2","sqlite3_vfs_register","sqlite3_db_filename","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t* f_data, size_t f_size) {
	if(f_size<=33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    // Create an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Create a window function
    sqlite3_create_window_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    
    // Set a trace callback function
    sqlite3_trace_v2(db, fuzz_uint32_t_4, [](unsigned int mask, void *ctx, void *p, void *x) -> int {
        // Trace callback logic
        return 0;
    }, nullptr);
    
    // Register a custom VFS (Virtual File System)
    sqlite3_vfs *vfs;
    sqlite3_vfs_register(vfs, fuzz_int32_t_5);
    
    // Get the database filename
    sqlite3_filename filename = sqlite3_db_filename(db, "main");
    
    // Open an input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
    
    // Open an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get the file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    
    // Release all allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}