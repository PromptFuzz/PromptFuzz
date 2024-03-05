#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 941
//<Prompt> ["cJSON_IsRaw","cJSON_AddItemReferenceToArray","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateStringReference"]
/*<Combination>: [cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_CreateFalse(),
    cJSON_bool cJSON_IsFalse(const cJSON *const item),
    cJSON *cJSON_CreateStringReference(const char * string)
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"cJSON_CreateStringReference":[[2475,9,2475,21,0,0,4,0]]},"library_calls":["cJSON_CreateStringReference","cJSON_IsRaw","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateStringReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateStringReference","cJSON_IsRaw","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateStringReference","cJSON_CreateArray","cJSON_AddItemReferenceToArray","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create FILE variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 1: Check if input data is a cJSON raw item
    cJSON *rawItem = cJSON_CreateStringReference("http:/*www.example.com/image/481989943");
    cJSON_bool isRaw = cJSON_IsRaw(rawItem);

    // Step 1: Create cJSON false item
    cJSON *falseItem = cJSON_CreateFalse();
    cJSON_bool isFalse = cJSON_IsFalse(falseItem);

    // Step 1: Create cJSON string reference item
    cJSON *stringItem = cJSON_CreateStringReference("View from 15th Floor");

    // Step 2: Add item reference to array
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(array, rawItem);

    // Step 3: Print the JSON data
    char *jsonString = cJSON_Print(array);
    printf("JSON data: %s\n", jsonString);

    // Step 5: Get file descriptor for reading and writing
    int inFileno = fuzz_fileno(in_file);
    int outFileno = fuzz_fileno(stdout);

    // Step 6: Directly use file name as string
    const char *outputFileName = "output_file";

    // Step 7: Release allocated resources
    cJSON_Delete(rawItem);
    cJSON_Delete(falseItem);
    cJSON_Delete(stringItem);
    cJSON_Delete(array);
    free(jsonString);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(outFileno);
	assert_fd_closed(inFileno);
	return 0;
}