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
//<ID> 932
//<Prompt> ["cJSON_IsRaw","cJSON_AddItemReferenceToArray","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateStringReference"]
/*<Combination>: [cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_CreateFalse(),
    cJSON_bool cJSON_IsFalse(const cJSON *const item),
    cJSON *cJSON_CreateStringReference(const char * string)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"cJSON_IsRaw":[[2978,9,2978,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_IsRaw","cJSON_CreateFalse","cJSON_IsFalse","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_IsRaw","cJSON_CreateFalse","cJSON_IsFalse","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Create a string for the input file name
    const char *input_file = "input_file";
    
    // Step 1: Check if the input is a raw cJSON item
    cJSON *root = cJSON_ParseWithLengthOpts((const char *)data, size, NULL, fuzz_int32_t_1);
    cJSON_bool isRaw = cJSON_IsRaw(root);
    
    // Step 2: Create a cJSON false item
    cJSON *falseItem = cJSON_CreateFalse();
    
    // Step 3: Check if the cJSON item is false
    cJSON_bool isFalse = cJSON_IsFalse(falseItem);
    
    // Step 5: Create a file descriptor for writing
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);
    
    // Step 7: Release all allocated resources
    cJSON_Delete(root);
    cJSON_Delete(falseItem);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}