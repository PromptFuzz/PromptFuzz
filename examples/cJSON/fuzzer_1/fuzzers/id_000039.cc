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
//<ID> 1142
//<Prompt> ["cJSON_CreateIntArray","cJSON_ParseWithLengthOpts","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddItemReferenceToObject","cJSON_CreateRaw","cJSON_ParseWithOpts","cJSON_ReplaceItemViaPointer"]
/*<Combination>: [cJSON *cJSON_CreateIntArray(const int * numbers, int count),
    cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    void cJSON_DeleteItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_AddItemReferenceToObject(cJSON * object, const char * string, cJSON * item),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON_bool cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, cJSON * replacement)
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2299,9,2299,28,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_ReplaceItemViaPointer","cJSON_GetObjectItem","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_1, fuzz_int32_t_array_size_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	
		const int * fuzzer_var_cJSON_CreateIntArray_1_0 = fuzz_int32_t_array_1;
		int fuzzer_size_cJSON_CreateIntArray_1_0 = static_cast<int>(fuzz_int32_t_array_size_1);//fuzzer shim end}




    // Create a file stream for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file stream
    char input_buffer[1024];
    if (fgets(input_buffer, sizeof(input_buffer), in_file) == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data as a cJSON array
    cJSON *root = cJSON_Parse(input_buffer);
    if (root == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a new cJSON array and add it to the root object
    int numbers[] = {1, 2, 3, 4, 5};
    cJSON *new_array = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_1_0, fuzzer_size_cJSON_CreateIntArray_1_0);
    cJSON_AddItemToObject(root, fuzz_str_2, new_array);

    // Replace an item in the root object with the new array using a pointer
    cJSON_ReplaceItemViaPointer(root, cJSON_GetObjectItem(root, fuzz_str_3), new_array);

    // Delete an item from the root object by its string key (case sensitive)
    cJSON_DeleteItemFromObjectCaseSensitive(root, fuzz_str_4);

    // Print the modified JSON as a formatted string
    char *output_string = cJSON_Print(root);
    if (output_string == NULL) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a file stream for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        free(output_string);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the output string to the file stream
    fputs(output_string, out_file);

    // Release resources
    cJSON_Delete(root);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(output_string);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}