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
//<ID> 334
//<Prompt> ["cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_IsTrue","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 28, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"cJSON_strdup":[[193,9,193,23,0,0,4,0]],"cJSON_CreateString":[[2462,12,2462,30,0,0,4,0]],"cJSON_CreateStringArray":[[2682,12,2682,14,0,0,4,0]]},"library_calls":["cJSON_CreateBool","cJSON_CreateArray","cJSON_InsertItemInArray","cJSON_CreateStringArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_CreateBool","cJSON_CreateArray","cJSON_InsertItemInArray","cJSON_CreateStringArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Create a cJSON boolean item
    cJSON *boolItem = cJSON_CreateBool(fuzz_int32_t_1);
    
    // Step 2: Create a cJSON array and insert the boolean item at index 0
    cJSON *array = cJSON_CreateArray();
    cJSON_InsertItemInArray(array, 0, boolItem);
    
    // Step 5: Get the file descriptor of the input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 3: Replace the boolean item in the array with a new cJSON string array
    cJSON *stringItem = cJSON_CreateStringArray((const char *const *)array, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(array, fuzz_str_2, stringItem);
    
    // Step 6: Use the output file name "output_file"
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 7: Write the cJSON object to the output file
    char *jsonStr = cJSON_Print(array);
    if (jsonStr != NULL) {
        fwrite(jsonStr, strlen(jsonStr), 1, out_file);
        free(jsonStr);
    }
    
    // Clean up resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cJSON_Delete(array);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}