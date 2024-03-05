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
//<ID> 429
//<Prompt> ["cJSON_IsBool","cJSON_AddBoolToObject","cJSON_AddNullToObject","cJSON_GetStringValue","cJSON_AddItemToObjectCS","cJSON_malloc","cJSON_CreateArray"]
/*<Combination>: [cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name),
    char *cJSON_GetStringValue(const cJSON *const item),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_CreateArray()
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["cJSON_CreateObject","cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_GetStringValue","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToObjectCS","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a cJSON object to hold the parsed JSON data
    cJSON *root = cJSON_CreateObject();
    
    // Open the input file as a FILE stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Read the input JSON data from the file
    char *input = new char[size + 1];
    fread(input, 1, size, in_file);
    input[size] = '\0';
    
    // Close the input file stream
    assert_file_closed(&in_file);;
    
    // Parse the input JSON data
    cJSON *parsed_json = cJSON_Parse(input);
    delete[] input;
    
    if (parsed_json) {
        // Check if the parsed JSON data is an array
        if (cJSON_IsArray(parsed_json)) {
            // Get the size of the array
            int array_size = cJSON_GetArraySize(parsed_json);
            
            // Iterate over each item in the array
            for (int i = 0; i < array_size; i++) {
                // Get the item at the current index
                cJSON *item = cJSON_GetArrayItem(parsed_json, fuzz_int32_t_1);
                
                // Check if the item is a string
                if (cJSON_IsString(item)) {
                    // Get the string value of the item
                    const char *str_value = cJSON_GetStringValue(item);
                    
                    // Create a new cJSON object to hold the string value
                    cJSON *str_obj = cJSON_CreateObject();
                    
                    // Add the string value to the object
                    cJSON_AddItemToObject(str_obj, fuzz_str_2, cJSON_CreateString(str_value));
                    
                    // Add the object to the root object
                    cJSON_AddItemToObjectCS(root, fuzz_str_3, str_obj);
                }
            }
        }
        
        // Print the root object to a string
        char *output = cJSON_PrintUnformatted(root);
        
        // Open the output file as a FILE stream
        FILE *out_file = fopen("output_file", "wb");
        
        // Write the output JSON data to the file
        fwrite(output, 1, strlen(output), out_file);
        
        // Close the output file stream
        assert_file_closed(&out_file);;
        
        // Free the output string
        cJSON_free(output);
    }
    
    // Delete the parsed JSON object
    cJSON_Delete(parsed_json);
    
    // Delete the root object
    cJSON_Delete(root);
    
    assert_file_closed(&in_file);
	return 0;
}