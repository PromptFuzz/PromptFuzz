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
//<ID> 1858
//<Prompt> ["cJSON_CreateFalse","cJSON_CreateNull","cJSON_ParseWithLength","cJSON_CreateTrue","cJSON_IsBool","cJSON_GetObjectItemCaseSensitive","cJSON_CreateIntArray"]
/*<Combination>: [cJSON *cJSON_CreateFalse(),
    cJSON *cJSON_CreateNull(),
    cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON *cJSON_CreateTrue(),
    cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_GetObjectItemCaseSensitive(const cJSON *const object, const char *const string),
    cJSON *cJSON_CreateIntArray(const int * numbers, int count)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"cJSON_IsBool":[[2919,9,2919,21,0,0,4,0]]},"library_calls":["cJSON_CreateFalse","cJSON_CreateNull","cJSON_CreateTrue","cJSON_ParseWithLength","cJSON_IsBool","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateFalse","cJSON_CreateNull","cJSON_CreateTrue","cJSON_ParseWithLength","cJSON_IsBool","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set input file name
    const char *in_file_name = "input_file";

    // Step 1: Create cJSON objects
    cJSON *false_obj = cJSON_CreateFalse();
    cJSON *null_obj = cJSON_CreateNull();
    cJSON *true_obj = cJSON_CreateTrue();

    // Step 2: Parse cJSON input data
    cJSON *input_array = cJSON_ParseWithLength((const char *)data, size);

    // Step 3: Check if parsed item is a boolean
    cJSON_bool is_bool = cJSON_IsBool(input_array);

    // Step 7: Release allocated resources
    cJSON_Delete(false_obj);
    cJSON_Delete(null_obj);
    cJSON_Delete(true_obj);
    cJSON_Delete(input_array);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}