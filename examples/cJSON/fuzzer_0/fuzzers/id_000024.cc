#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 431
//<Prompt> ["cJSON_IsBool","cJSON_AddBoolToObject","cJSON_AddNullToObject","cJSON_GetStringValue","cJSON_AddItemToObjectCS","cJSON_malloc","cJSON_CreateArray"]
/*<Combination>: [cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name),
    char *cJSON_GetStringValue(const cJSON *const item),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_CreateArray()
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"cJSON_AddNullToObject":[[2082,9,2082,69,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsBool","cJSON_IsTrue","cJSON_AddBoolToObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_AddNullToObject","cJSON_CreateArray","cJSON_AddItemToObjectCS","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_IsBool","cJSON_IsTrue","cJSON_AddBoolToObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetStringValue","cJSON_AddNullToObject","cJSON_CreateArray","cJSON_AddItemToObjectCS","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete"],"visited":2}
/*Here is the C++ program that achieves the event using the cJSON library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file stream for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data using cJSON_ParseWithOpts
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    rewind(in_file);
    char *json_data = (char *)malloc(file_size + 1);
    if (json_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(json_data, 1, file_size, in_file);
    assert_file_closed(&in_file);;
    json_data[file_size] = '\0';
    cJSON *root = cJSON_Parse(json_data);
    free(json_data);

    // Verify if the parsed root object is valid
    if (root == NULL || cJSON_IsInvalid(root)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Use cJSON_IsBool to determine if an item in the root object is a boolean
    cJSON *bool_item = cJSON_GetObjectItem(root, "useJSP");
    if (bool_item != NULL && cJSON_IsBool(bool_item)) {
        cJSON_bool use_jsp = cJSON_IsTrue(bool_item);
        // Use cJSON_AddBoolToObject to add a boolean item to the root object
        cJSON_AddBoolToObject(root, "useJSP-duplicate", use_jsp);
    }

    // Use cJSON_GetStringValue to get the string value of an item in the root object
    cJSON *string_item = cJSON_GetObjectItem(root, "defaultListTemplate");
    if (string_item != NULL && cJSON_IsString(string_item)) {
        const char *template_name = cJSON_GetStringValue(string_item);
        if (template_name != NULL) {
            // Use cJSON_AddNullToObject to add a null item to the root object
            cJSON_AddNullToObject(root, "defaultListTemplate-duplicate");
        }
    }

    // Use cJSON_AddItemToObjectCS to add an item to the root object with a case-sensitive key
    cJSON *array = cJSON_CreateArray();
    if (array != NULL) {
        cJSON_AddItemToObjectCS(root, "newArray", array);
    }

    // Use cJSON_malloc to allocate memory for an item in the array
    cJSON *number_item = cJSON_CreateNumber(10);
    if (number_item != NULL) {
        cJSON_AddItemToArray(array, number_item);
    }

    // Generate the modified JSON string
    char *modified_json = cJSON_Print(root);
    if (modified_json != NULL) {
        printf("Modified JSON: %s\n", modified_json);
        free(modified_json);
    }

    // Cleanup resources
    cJSON_Delete(root);

    assert_file_closed(&in_file);
	return 0;
}