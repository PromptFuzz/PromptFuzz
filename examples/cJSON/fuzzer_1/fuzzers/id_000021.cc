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
//<ID> 410
//<Prompt> ["cJSON_IsBool","cJSON_AddBoolToObject","cJSON_AddNullToObject","cJSON_GetStringValue","cJSON_AddItemToObjectCS","cJSON_malloc","cJSON_CreateArray"]
/*<Combination>: [cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name),
    char *cJSON_GetStringValue(const cJSON *const item),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_CreateArray()
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"cJSON_AddBoolToObject":[[2118,9,2118,74,0,0,4,1]],"add_item_to_object":[[2016,29,2016,45,0,0,4,0]],"cJSON_ParseWithOpts":[[1083,9,1083,22,0,0,4,0]]},"library_calls":["cJSON_CreateObject","cJSON_Delete","cJSON_Delete","cJSON_Parse","cJSON_Delete","cJSON_IsBool","cJSON_IsTrue","cJSON_AddBoolToObject","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_Parse","cJSON_IsBool","cJSON_IsTrue","cJSON_AddBoolToObject","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a cJSON object to hold the parsed data
    cJSON *root = cJSON_CreateObject();

    // Create a file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file
    char *input_data = nullptr;
    size_t input_data_size = 0;
    char line[256];
    while (fgets(line, sizeof(line), in_file) != nullptr) {
        size_t len = strlen(line);
        if (len > 0) {
            // Remove the newline character at the end of the line
            if (line[len - 1] == '\n') {
                line[len - 1] = '\0';
                len--;
            }
            // Append the line to the input data
            char *new_data = (char *)realloc(input_data, input_data_size + len + 1);
            if (new_data == nullptr) {
                free(input_data);
                cJSON_Delete(root);
                assert_file_closed(&in_file);;
                assert_file_closed(&in_file);
	return 0;
            }
            input_data = new_data;
            memcpy(input_data + input_data_size, line, len);
            input_data_size += len;
            input_data[input_data_size] = '\0';
        }
    }

    // Close the input file
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_Parse
    cJSON *parsed_data = cJSON_Parse(input_data);
    if (parsed_data == nullptr) {
        free(input_data);
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Iterate over the parsed data and add each item to the root object
    cJSON *item = nullptr;
    cJSON_ArrayForEach(item, parsed_data) {
        // Check if the item is a boolean
        if (cJSON_IsBool(item)) {
            // Get the boolean value
            cJSON_bool value = cJSON_IsTrue(item) ? 1 : 0;
            // Get the item's key (name)
            const char *key = item->string;
            // Add the boolean item to the root object
            cJSON_AddBoolToObject(root, key, value);
        }
    }

    // Create a file to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        free(input_data);
        cJSON_Delete(parsed_data);
        cJSON_Delete(root);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Convert the root object to a string and write it to the output file
    char *output_data = cJSON_Print(root);
    if (output_data != nullptr) {
        fputs(output_data, out_file);
        free(output_data);
    }

    // Close the output file
    assert_file_closed(&out_file);;

    // Free allocated memory
    free(input_data);
    cJSON_Delete(parsed_data);
    cJSON_Delete(root);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}