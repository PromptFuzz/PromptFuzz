#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 141
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
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":27,"unique_branches":{"cJSON_SetNumberHelper":[[384,9,384,26,0,0,4,0],[388,14,388,39,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_Delete","cJSON_GetArraySize","cJSON_CreateObject","cJSON_Delete","cJSON_GetArrayItem","cJSON_Delete","cJSON_Delete","cJSON_IsArray","cJSON_Delete","cJSON_Delete","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_Delete","cJSON_Delete","cJSON_IsNumber","cJSON_Delete","cJSON_Delete","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Delete","cJSON_free","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_CreateObject","cJSON_GetArrayItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_SetNumberHelper","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete","cJSON_Delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file stream for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file stream
    char input[1024];
    fgets(input, sizeof(input), in_file);

    // Close the file stream
    assert_file_closed(&in_file);;

    // Parse the input data as a cJSON array
    cJSON *root = cJSON_Parse(input);
    if (!root) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the root is an array
    if (!cJSON_IsArray(root)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the size of the array
    int arraySize = cJSON_GetArraySize(root);

    // Create a new cJSON object for output
    cJSON *output = cJSON_CreateObject();
    if (!output) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    // Iterate over each item in the array
    for (int i = 0; i < arraySize; i++) {
        // Get the current item in the array
        cJSON *item = cJSON_GetArrayItem(root, i);
        if (!item) {
            cJSON_Delete(root);
            cJSON_Delete(output);
            assert_file_closed(&in_file);
	return 0;
        }

        // Check if the item is an array
        if (!cJSON_IsArray(item)) {
            cJSON_Delete(root);
            cJSON_Delete(output);
            assert_file_closed(&in_file);
	return 0;
        }

        // Get the size of the inner array
        int innerArraySize = cJSON_GetArraySize(item);

        // Iterate over each inner item in the array
        for (int j = 0; j < innerArraySize; j++) {
            // Get the current inner item in the array
            cJSON *innerItem = cJSON_GetArrayItem(item, j);
            if (!innerItem) {
                cJSON_Delete(root);
                cJSON_Delete(output);
                assert_file_closed(&in_file);
	return 0;
            }

            // Check if the inner item is a number
            if (!cJSON_IsNumber(innerItem)) {
                cJSON_Delete(root);
                cJSON_Delete(output);
                assert_file_closed(&in_file);
	return 0;
            }

            // Get the value of the inner item as a number
            double number = cJSON_GetNumberValue(innerItem);

            // Set the value of the output object using the number as the key and value
            cJSON_SetNumberHelper(output, number);
        }
    }

    // Print the output object
    char *outputString = cJSON_PrintUnformatted(output);
    if (!outputString) {
        cJSON_Delete(root);
        cJSON_Delete(output);
        assert_file_closed(&in_file);
	return 0;
    }

    // Print the output string
    printf("%s\n", outputString);

    // Free the output string
    cJSON_free(outputString);

    // Delete the cJSON objects
    cJSON_Delete(root);
    cJSON_Delete(output);

    assert_file_closed(&in_file);
	return 0;
}