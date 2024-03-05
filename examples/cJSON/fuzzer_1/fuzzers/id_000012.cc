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
//<ID> 142
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
//<score> 22.5, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"cJSON_IsTrue":[[2908,9,2908,21,0,0,4,1]],"cJSON_IsInvalid":[[2888,9,2888,21,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_GetArrayItem","cJSON_Delete","cJSON_GetArraySize","cJSON_Delete","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_IsInvalid","cJSON_PrintUnformatted","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetArrayItem","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_AddTrueToObject","cJSON_IsTrue","cJSON_GetObjectItem","cJSON_IsInvalid","cJSON_PrintUnformatted","cJSON_Delete"],"visited":1}
/*Here is the C++ program that uses the cJSON library APIs to achieve the event specified:

*/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Create a file for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data using cJSON_Parse
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    char* buffer = (char*)malloc(file_size + 1);
    fread(buffer, file_size, 1, in_file);
    buffer[file_size] = '\0';
    cJSON* root = cJSON_Parse(buffer);
    free(buffer);
    assert_file_closed(&in_file);;
    if (root == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the array from the root object
    cJSON* array = cJSON_GetArrayItem(root, fuzz_int32_t_1);
    if (array == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the size of the array
    int array_size = cJSON_GetArraySize(array);
    if (array_size == 0) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Iterate through the array and perform operations on each element
    for (int i = 0; i < array_size; i++) {
        cJSON* item = cJSON_GetArrayItem(array, fuzz_int32_t_2);

        // Check if the item is a number
        if (cJSON_IsNumber(item)) {
            double number = cJSON_GetNumberValue(item);

            // Set the number to a new value
            cJSON_SetNumberHelper(item, number * 2);
        }
    }

    // Add a true value to the root object with the name "success"
    cJSON_AddTrueToObject(root, fuzz_str_3);

    // Create a bool variable and check if it is true in cJSON
    cJSON_bool success = cJSON_IsTrue(cJSON_GetObjectItem(root, fuzz_str_4));

    // Check if any invalid items exist in the root object
    cJSON_bool hasInvalidItems = cJSON_IsInvalid(root);

    // Print the modified cJSON structure to a string
    char* output = cJSON_PrintUnformatted(root);

    // Print the output string to stdout for fuzzing engine to collect
    printf("%s\n", output);

    // Free the resources
    cJSON_Delete(root);
    free(output);

    assert_file_closed(&in_file);
	return 0;
}