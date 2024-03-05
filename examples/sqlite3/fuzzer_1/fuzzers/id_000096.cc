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
//<ID> 2504
//<Prompt> ["sqlite3_bind_blob","sqlite3_soft_heap_limit","sqlite3_open16","sqlite3_column_blob","sqlite3_rtree_query_callback"]
/*<Combination>: [int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    void sqlite3_soft_heap_limit(int N),
    int sqlite3_open16(const void * filename, sqlite3 ** ppDb),
    const void *sqlite3_column_blob(sqlite3_stmt * , int iCol),
    int sqlite3_rtree_query_callback(sqlite3 * db, const char * zQueryFunc, int (*)(sqlite3_rtree_query_info *) xQueryFunc, void * pContext, void (*)(void *) xDestructor)
*/
//<score> 0.40000004, nr_unique_branch: 0, p_cov: 0.6
//<Quality> {"diversity":6,"density":4,"covered_api":["sqlite3_bind_blob","sqlite3_soft_heap_limit","sqlite3_column_blob"],"uncovered_api":["sqlite3_open16","sqlite3_rtree_query_callback"],"unique_branches":{},"library_calls":["sqlite3_soft_heap_limit","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_bind_blob","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create input and output file pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Define file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Step 2: Call the required sqlite3 APIs
    sqlite3_stmt *stmt;
    sqlite3 *db;
    sqlite3_soft_heap_limit(fuzz_int32_t_1); // Limit the heap size
   
    // Step 1: Binding blob data to a prepared statement
    const void *blob_data = sqlite3_column_blob(stmt, 0);
    int blob_size = sqlite3_column_bytes(stmt, fuzz_int32_t_2);
    sqlite3_bind_blob(stmt, fuzz_int32_t_3, blob_data, blob_size, SQLITE_STATIC);
    
    // Step 7: Release allocated resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}