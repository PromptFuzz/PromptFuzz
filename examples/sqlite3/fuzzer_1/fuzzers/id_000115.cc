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
//<ID> 2987
//<Prompt> ["sqlite3_os_init","sqlite3_extended_errcode","sqlite3_overload_function","sqlite3_realloc64","sqlite3_column_type"]
/*<Combination>: [int sqlite3_os_init(),
    int sqlite3_extended_errcode(sqlite3 * db),
    int sqlite3_overload_function(sqlite3 * , const char * zFuncName, int nArg),
    void *sqlite3_realloc64(void * , sqlite3_uint64 ),
    int sqlite3_column_type(sqlite3_stmt * , int iCol)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":4,"density":2,"covered_api":["sqlite3_os_init","sqlite3_extended_errcode","sqlite3_overload_function","sqlite3_column_type"],"uncovered_api":["sqlite3_realloc64"],"unique_branches":{"sqlite3_overload_function":[[178661,7,178661,32,0]]},"library_calls":["sqlite3_os_init","sqlite3_extended_errcode","sqlite3_overload_function","sqlite3_column_type"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t* f_data, size_t f_size) {
	if(f_size<=22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        // Handle error
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    if (in_fd == -1) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    if (out_fd == -1) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 1: Initialize sqlite3 OS
    int os_init_result = sqlite3_os_init();
    
    // Step 2: Get extended error code
    sqlite3 *db = nullptr;
    int extended_errcode = sqlite3_extended_errcode(db);
    
    // Step 3: Overload function
    const char *funcName = "my_function";
    int nArg = 2;
    int overload_result = sqlite3_overload_function(db, fuzz_str_1, fuzz_int32_t_2);
    
    // Step 2: Get column type
    sqlite3_stmt *stmt = nullptr;
    int iCol = 0;
    int column_type = sqlite3_column_type(stmt, fuzz_int32_t_3);
    
    // Step 1: Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}