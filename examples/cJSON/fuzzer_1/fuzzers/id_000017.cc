#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 309
//<Prompt> ["cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_IsTrue","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 15, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2294,9,2294,25,0,0,4,0]],"cJSON_CreateStringArray":[[2672,24,2672,41,0,0,4,0]]},"library_calls":["cJSON_CreateStringArray","cJSON_CreateTrue","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateStringArray","cJSON_CreateTrue","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<7) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Check if the file was opened successfully
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create a cJSON boolean variable
    cJSON_bool boolean = cJSON_True;

    // Step 5: Get the file descriptor for reading
    int in_fileno = fuzz_fileno(in_file);

    // Step 2: Create a cJSON array
    cJSON *array = cJSON_CreateStringArray(nullptr, 0);

    // Step 3: Replace an item in the array (if exists)
    cJSON *newitem = cJSON_CreateTrue();
    cJSON_ReplaceItemInObjectCaseSensitive(array, fuzz_str_1, newitem);

    // Step 6: Use a constant string as the output file name
    const char *output_file = "output_file";

    // Step 4: Create a FILE * variable to write output data
    FILE *out_file = fopen(output_file, "wb");

    // Check if the file was opened successfully
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fileno);
	return 0;
    }

    // Step 5: Get the file descriptor for writing
    int out_fileno = fuzz_fileno(out_file);

    // Step 7: Release all allocated resources before return
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cJSON_Delete(newitem);
    cJSON_Delete(array);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fileno);
	assert_fd_closed(in_fileno);
	return 0;
}