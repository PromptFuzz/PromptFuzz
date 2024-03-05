#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1415
//<Prompt> ["cJSON_DetachItemFromObjectCaseSensitive","cJSON_PrintBuffered","cJSON_PrintPreallocated","cJSON_ParseWithOpts","cJSON_CreateTrue","cJSON_CreateFloatArray","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format),
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON *cJSON_CreateTrue(),
    cJSON *cJSON_CreateFloatArray(const float * numbers, int count),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"add_item_to_object":[[2037,48,2037,70,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_DetachItemFromObjectCaseSensitive","cJSON_PrintBuffered","cJSON_free","cJSON_AddItemToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_DetachItemFromObjectCaseSensitive","cJSON_PrintBuffered","cJSON_free","cJSON_AddItemToObject","cJSON_CreateTrue","cJSON_AddItemToObject","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_AddItemToObject","cJSON_PrintPreallocated","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *input_data = (char *)malloc(file_size + 1);
    if (input_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(input_data, 1, file_size, in_file);
    input_data[file_size] = '\0';

    // Close the input file
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_ParseWithOpts
    cJSON *root = cJSON_ParseWithOpts(input_data, NULL, 0);
    if (root == NULL) {
        free(input_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Detach an item from the object using cJSON_DetachItemFromObjectCaseSensitive
    cJSON *item = cJSON_DetachItemFromObjectCaseSensitive(root, "widget");
    if (item != NULL) {
        // Print the detached item using cJSON_PrintBuffered
        int prebuffer = 0;
        cJSON_bool fmt = cJSON_False;
        char *item_string = cJSON_PrintBuffered(item, prebuffer, fmt);
        if (item_string != NULL) {
            // Write the item string to an output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file != NULL) {
                fwrite(item_string, strlen(item_string), 1, out_file);
                assert_file_closed(&out_file);;
            }
            cJSON_free(item_string);
        }

        // Reattach the item to the object
        cJSON_AddItemToObject(root, "widget", item);
    }

    // Create a cJSON true value using cJSON_CreateTrue
    cJSON *true_value = cJSON_CreateTrue();
    if (true_value != NULL) {
        // Add the true value to the root object
        cJSON_AddItemToObject(root, "true_value", true_value);
    }

    // Create a cJSON float array using cJSON_CreateFloatArray
    float float_array[] = {1.0f, 2.0f, 3.0f};
    int float_array_count = sizeof(float_array) / sizeof(float);
    cJSON *float_array_value = cJSON_CreateFloatArray(float_array, float_array_count);
    if (float_array_value != NULL) {
        // Add the float array to the root object
        cJSON_AddItemToObject(root, "float_array", float_array_value);
    }

    // Create a cJSON string array using cJSON_CreateStringArray
    const char *string_array[] = {"string1", "string2", "string3"};
    int string_array_count = sizeof(string_array) / sizeof(const char *);
    cJSON *string_array_value = cJSON_CreateStringArray(string_array, string_array_count);
    if (string_array_value != NULL) {
        // Add the string array to the root object
        cJSON_AddItemToObject(root, "string_array", string_array_value);
    }

    // Print the modified root object using cJSON_PrintPreallocated
    cJSON_bool format = cJSON_True;
    int buffer_size = 1024;
    char *buffer = (char *)malloc(buffer_size);
    if (buffer != NULL) {
        cJSON_PrintPreallocated(root, buffer, buffer_size, format);
        free(buffer);
    }

    // Delete the root object
    cJSON_Delete(root);

    // Free the input data
    free(input_data);

    assert_file_closed(&in_file);
	return 0;
}