#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 501
//<Prompt> ["cJSON_CreateFloatArray","cJSON_CreateArrayReference","cJSON_AddItemToArray","cJSON_AddRawToObject","cJSON_malloc","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_HasObjectItem"]
/*<Combination>: [cJSON *cJSON_CreateFloatArray(const float * numbers, int count),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_Duplicate(const cJSON * item, cJSON_bool recurse),
    cJSON_bool cJSON_AddItemToObject(cJSON * object, const char * string, cJSON * item),
    cJSON_bool cJSON_HasObjectItem(const cJSON * object, const char * string)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["cJSON_CreateArray","cJSON_Parse","cJSON_IsArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateArray","cJSON_Parse","cJSON_IsArray","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here is a C++ program that uses the cJSON library to achieve the event specified in step 1:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file stream from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Read the input data as a JSON string
    char *input_data = new char[size + 1];
    fread(input_data, 1, size, in_file);
    input_data[size] = '\0';

    // Create a cJSON array to store the parsed JSON
    cJSON *root_array = cJSON_CreateArray();
    
    // Parse the input JSON data
    cJSON *parsed_json = cJSON_Parse(input_data);
    
    // Check if parsing is successful
    if (parsed_json != NULL) {
        // Check if the parsed JSON is an array
        if (cJSON_IsArray(parsed_json)) {
            // Iterate over each item in the array
            cJSON *item = NULL;
            cJSON_ArrayForEach(item, parsed_json) {
                // Create a duplicate of the item
                cJSON *item_duplicate = cJSON_Duplicate(item, cJSON_True);
                
                // Add the item duplicate to the root array
                cJSON_AddItemToArray(root_array, item_duplicate);
            }
        }
    }
    
    // Print the modified JSON to a string
    char *output_data = cJSON_Print(root_array);
    
    // Write the output JSON string to a file
    FILE *out_file = fopen("output_file", "wb");
    fwrite(output_data, 1, strlen(output_data), out_file);
    
    // Clean up resources
    cJSON_Delete(parsed_json);
    cJSON_Delete(root_array);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    delete[] input_data;
    free(output_data);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}