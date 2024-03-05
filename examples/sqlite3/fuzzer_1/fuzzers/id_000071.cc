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
//<ID> 1735
//<Prompt> ["sqlite3_blob_read","sqlite3_result_error","sqlite3_bind_blob","sqlite3_vmprintf","sqlite3_file_control"]
/*<Combination>: [int sqlite3_blob_read(sqlite3_blob * , void * Z, int N, int iOffset),
    void sqlite3_result_error(sqlite3_context * , const char * , int ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    int sqlite3_file_control(sqlite3 * , const char * zDbName, int op, void * )
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":1,"density":1,"covered_api":["sqlite3_file_control"],"uncovered_api":["sqlite3_blob_read","sqlite3_result_error","sqlite3_bind_blob","sqlite3_vmprintf"],"unique_branches":{"sqlite3_file_control":[[180473,7,180473,32,0]]},"library_calls":["sqlite3_file_control"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<=14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Use the file descriptor as needed
    // Example: pass the file descriptor to sqlite3_file_control API
    sqlite3_file_control(nullptr, fuzz_str_1, fuzz_int32_t_2, (void *)&in_fd);

    // Step 7: Release the resources
    assert_file_closed(&in_file);;

    // Return 0 indicating successful execution
    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}