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
//<ID> 84
//<Prompt> ["cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_DetachItemFromArray","cJSON_PrintPreallocated"]
/*<Combination>: [cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_IsNull(const cJSON *const item),
    cJSON_bool cJSON_IsArray(const cJSON *const item),
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string),
    cJSON *cJSON_DetachItemFromObject(cJSON * object, const char * string),
    cJSON *cJSON_DetachItemFromArray(cJSON * array, int which),
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format)
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromObject","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_CreateString","cJSON_AddItemToObject","cJSON_DetachItemFromObject","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from in_file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *input_data = (char *)malloc(file_size + 1);
    fread(input_data, file_size, 1, in_file);
    input_data[file_size] = '\0';

    // JSON parsing
    cJSON *root = cJSON_Parse(input_data);
    free(input_data);
    assert_file_closed(&in_file);;
    if (root == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the root is an array
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the array size
    int array_size = cJSON_GetArraySize(root);

    // Iterate over the array elements
    for (int i = 0; i < array_size; i++) {
        cJSON *array_item = cJSON_GetArrayItem(root, fuzz_int32_t_1);
        if (cJSON_IsArray(array_item)) {
            // Detach an item from the array
            cJSON *detached_item = cJSON_DetachItemFromArray(root, fuzz_int32_t_2);
            // Free the detached item
            cJSON_Delete(detached_item);
        }
    }

    // Create a new string object
    cJSON *string_item = cJSON_CreateString(fuzz_str_3);
    // Add the string object to the root object
    cJSON_AddItemToObject(root, fuzz_str_4, string_item);

    // Detach an item from the root object
    cJSON *detached_item = cJSON_DetachItemFromObject(root, fuzz_str_5);

    // Print the detached item to a buffer
    char buffer[256];
    cJSON_PrintPreallocated(detached_item, buffer, sizeof(buffer), fuzz_int32_t_6);

    // Write the buffer to an output file
    FILE *out_file = fopen("output_file", "wb");
    fwrite(buffer, strlen(buffer), 1, out_file);
    assert_file_closed(&out_file);;

    // Free the detached item and root object
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}