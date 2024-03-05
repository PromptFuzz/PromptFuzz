#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1508
//<Prompt> ["cJSON_Compare","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetNumberValue","cJSON_AddBoolToObject","cJSON_InsertItemInArray"]
/*<Combination>: [cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    double cJSON_GetNumberValue(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem)
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_GetArrayItem","cJSON_IsArray","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetArrayItem","cJSON_IsArray","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    // Create a FILE * variable for reading input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data from the file
    char* input_data = new char[size + 1];
    fread(input_data, sizeof(char), size, in_file);
    input_data[size] = '\0'; // Add null terminator
    
    // Close the input file and release resources
    assert_file_closed(&in_file);;
    
    // Parse the input JSON data
    cJSON* root = cJSON_Parse(input_data);
    if (root == nullptr) {
        delete[] input_data;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Perform the desired operation using cJSON APIs
    cJSON* array = cJSON_GetArrayItem(root, 0);
    if (array != nullptr && cJSON_IsArray(array)) {
        cJSON* newItem = cJSON_CreateBool(true);
        cJSON_InsertItemInArray(array, 0, newItem);
    }
    
    // Get the modified JSON data as a string
    char* output_data = cJSON_Print(root);
    
    // Create a FILE * variable for writing output data
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        cJSON_Delete(root);
        delete[] input_data;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write the output data to the file
    fwrite(output_data, sizeof(char), strlen(output_data), out_file);
    
    // Close the output file and release resources
    assert_file_closed(&out_file);;
    
    // Release allocated resources
    cJSON_Delete(root);
    delete[] input_data;
    free(output_data);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}