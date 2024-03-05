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
//<ID> 850
//<Prompt> ["sqlite3_enable_shared_cache","sqlite3_result_zeroblob","sqlite3_vfs_find","sqlite3_interrupt","sqlite3_expired","sqlite3_vtab_rhs_value"]
/*<Combination>: [int sqlite3_enable_shared_cache(int ),
    void sqlite3_result_zeroblob(sqlite3_context * , int n),
    sqlite3_vfs *sqlite3_vfs_find(const char * zVfsName),
    void sqlite3_interrupt(sqlite3 * ),
    int sqlite3_expired(sqlite3_stmt * ),
    int sqlite3_vtab_rhs_value(sqlite3_index_info * , int , sqlite3_value ** ppVal)
*/
//<score> 0.5555556, nr_unique_branch: 1, p_cov: 0.8333333
//<Quality> {"diversity":6,"density":2,"covered_api":["sqlite3_enable_shared_cache","sqlite3_vfs_find","sqlite3_interrupt","sqlite3_expired","sqlite3_vtab_rhs_value"],"uncovered_api":["sqlite3_result_zeroblob"],"unique_branches":{"sqlite3_expired":[[89263,10,89263,14,0]]},"library_calls":["sqlite3_enable_shared_cache","sqlite3_vfs_find","sqlite3_interrupt","sqlite3_expired","sqlite3_vtab_rhs_value","sqlite3_free"]}
/*Here is the C++ program that uses the sqlite3 library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<=19) return 0;

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
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Get input file name
    const char *in_filename = "input_file";
    
    // Step 4: Open output file
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
    
    // Step 5: Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Get output file name
    const char *out_filename = "output_file";
    
    // Step 1: Enable shared cache
    sqlite3_enable_shared_cache(fuzz_int32_t_1);
    
    // Step 2: Call other APIs
    sqlite3_vfs *vfs = sqlite3_vfs_find(fuzz_str_2);
    
    sqlite3_interrupt(NULL);
    
    sqlite3_stmt *stmt;
    int expired = sqlite3_expired(stmt);
    
    sqlite3_index_info index_info;
    sqlite3_value *ppVal;
    sqlite3_vtab_rhs_value(&index_info, fuzz_int32_t_3, &ppVal);
    
    // Step 7: Release resources
    sqlite3_free(stmt);
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