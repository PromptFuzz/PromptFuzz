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
//<ID> 232
//<Prompt> ["cJSON_CreateArrayReference","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_AddTrueToObject","cJSON_CreateBool","cJSON_IsTrue","cJSON_IsInvalid","cJSON_PrintUnformatted","cJSON_GetArraySize"]
/*<Combination>: [cJSON *cJSON_CreateArrayReference(const cJSON * child),
    double cJSON_GetNumberValue(const cJSON *const item),
    double cJSON_SetNumberHelper(cJSON * object, double number),
    cJSON *cJSON_AddTrueToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON_bool cJSON_IsInvalid(const cJSON *const item),
    char *cJSON_PrintUnformatted(const cJSON * item),
    int cJSON_GetArraySize(const cJSON * array)
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["cJSON_CreateArray","cJSON_CreateArrayReference","cJSON_GetArraySize","cJSON_SetNumberHelper","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateArray","cJSON_CreateArrayReference","cJSON_GetArraySize","cJSON_SetNumberHelper","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 1: Create an array and set its reference
    cJSON *array = cJSON_CreateArray();
    cJSON *arrayRef = cJSON_CreateArrayReference(array);

    // Step 2: Get the size of the array and print it
    int arraySize = cJSON_GetArraySize(array);
    printf("Array size: %d\n", arraySize);

    // Step 3: Set a number to the array
    cJSON_SetNumberHelper(array, 42.0);

    // Step 5: Get the file descriptor of the input file
    int in_file_fd = fuzz_fileno(in_file);

    // Step 6: Use the string directly as the output file name
    const char *output_file = "output_file";

    // Step 7: Cleanup and release resources
    assert_file_closed(&in_file);;
    cJSON_Delete(array);
    cJSON_Delete(arrayRef);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_fd);
	return 0;
}