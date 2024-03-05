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
//<ID> 70
//<Prompt> ["sqlite3_str_reset","sqlite3_value_nochange","sqlite3_progress_handler","sqlite3_busy_timeout","sqlite3_shutdown"]
/*<Combination>: [void sqlite3_str_reset(sqlite3_str * ),
    int sqlite3_value_nochange(sqlite3_value * ),
    void sqlite3_progress_handler(sqlite3 * , int , int (*)(void *) , void * ),
    int sqlite3_busy_timeout(sqlite3 * , int ms),
    int sqlite3_shutdown()
*/
//<score> 1.7142859, nr_unique_branch: 4, p_cov: 0.6
//<Quality> {"diversity":7,"density":4,"covered_api":["sqlite3_str_reset","sqlite3_progress_handler","sqlite3_busy_timeout"],"uncovered_api":["sqlite3_value_nochange","sqlite3_shutdown"],"unique_branches":{"sqlite3_busy_timeout":[[178312,7,178312,32,1],[178314,7,178314,11,0]],"sqlite3_progress_handler":[[178286,7,178286,32,1],[178292,7,178292,13,0]]},"library_calls":["sqlite3_str_new","sqlite3_open","sqlite3_progress_handler","sqlite3_busy_timeout","sqlite3_str_reset","sqlite3_str_finish","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Create a sqlite3_str object
    sqlite3_str *strObj = sqlite3_str_new(nullptr);
    
    // Create a sqlite3 object
    sqlite3 *db = nullptr;
    sqlite3_open(":memory:", &db);
    
    // Set a progress handler
    sqlite3_progress_handler(db, fuzz_int32_t_1, [](void *) -> int { return 0; }, nullptr);
    
    // Set a busy timeout
    sqlite3_busy_timeout(db, fuzz_int32_t_2);
    
    // Reset the sqlite3_str object
    sqlite3_str_reset(strObj);
    
    // Read the input data from a file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);
    
    // Write the output data to a file
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);
    const char *out_file_name = "output_file";
    
    // Call other sqlite3 APIs if needed
    
    // Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_str_finish(strObj);
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}