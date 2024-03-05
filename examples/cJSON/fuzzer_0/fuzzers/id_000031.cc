#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 700
//<Prompt> ["cJSON_CreateObject","cJSON_CreateNumber","cJSON_ReplaceItemInObject","cJSON_ParseWithLength","cJSON_AddRawToObject"]
/*<Combination>: [cJSON *cJSON_CreateObject(),
    cJSON *cJSON_CreateNumber(double num),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw)
*/
//<score> 11, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"cJSON_ReplaceItemViaPointer":[[2294,54,2294,68,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_Delete","cJSON_ReplaceItemInObject","cJSON_Delete","cJSON_Delete","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_ReplaceItemInObject","cJSON_AddRawToObject","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Set up input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read input data from file
    fseek(in_file, 0, SEEK_END);
    long in_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *input_data = (char *)malloc(in_size + 1);
    fread(input_data, 1, in_size, in_file);
    assert_file_closed(&in_file);;
    input_data[in_size] = '\0';
    
    // Output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        free(input_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create cJSON object
    cJSON *root = cJSON_ParseWithLength(input_data, in_size);
    if (root == nullptr) {
        assert_file_closed(&out_file);;
        free(input_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a new cJSON object
    cJSON *new_item = cJSON_CreateObject();
    if (new_item == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&out_file);;
        free(input_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Replace item in JSON object
    cJSON_bool replace_result = cJSON_ReplaceItemInObject(root, "City", new_item);
    if (!replace_result) {
        cJSON_Delete(new_item);
        cJSON_Delete(root);
        assert_file_closed(&out_file);;
        free(input_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Add raw string to JSON object
    const char *raw_string = "Raw String";
    cJSON *raw_item = cJSON_AddRawToObject(root, "Raw", raw_string);
    if (raw_item == nullptr) {
        cJSON_Delete(new_item);
        cJSON_Delete(root);
        assert_file_closed(&out_file);;
        free(input_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Print the updated JSON object
    char *output_data = cJSON_Print(root);
    fprintf(out_file, "%s\n", output_data);
    
    // Release resources
    cJSON_Delete(root);
    assert_file_closed(&out_file);;
    free(output_data);
    free(input_data);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
