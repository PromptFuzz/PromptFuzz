#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 187
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
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_Print","cJSON_Delete"],"visited":1}
/*Here is the C++ program that achieves the given event using the cJSON library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create a FILE* variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create a cJSON object from the input data
    fseek(in_file, 0, SEEK_END);
    size_t length = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *json_data = (char *)malloc(length + 1);
    fread(json_data, 1, length, in_file);
    assert_file_closed(&in_file);;
    json_data[length] = '\0';
    cJSON *root = cJSON_Parse(json_data);
    free(json_data);

    // Check if parsing was successful
    if (root == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get the number of items in the root array
    cJSON *array = cJSON_GetObjectItem(root, "web-app");
    if (array == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }
    int array_size = cJSON_GetArraySize(array);

    // Step 3: Set a new number value for each item in the root array
    for (int i = 0; i < array_size; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);
        if (item != nullptr) {
            double number = cJSON_GetNumberValue(item);
            cJSON_SetNumberHelper(item, number + 1.0);
        }
    }

    // Print the modified cJSON object
    char *output = cJSON_PrintUnformatted(root);
    printf("%s\n", output);
    free(output);

    // Step 5: Create a FILE* variable for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the modified cJSON object to the output file
    char *formatted_output = cJSON_Print(root);
    if (formatted_output != nullptr) {
        fputs(formatted_output, out_file);
        free(formatted_output);
    }

    assert_file_closed(&out_file);;

    // Step 7: Release allocated resources
    cJSON_Delete(root);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}