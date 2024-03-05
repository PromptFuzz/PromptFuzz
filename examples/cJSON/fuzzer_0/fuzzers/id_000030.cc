#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 646
//<Prompt> ["cJSON_ParseWithOpts","cJSON_IsBool","cJSON_IsRaw","cJSON_IsNumber","cJSON_ReplaceItemInArray","cJSON_CreateArrayReference","cJSON_AddFalseToObject"]
/*<Combination>: [cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_IsNumber(const cJSON *const item),
    cJSON_bool cJSON_ReplaceItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name)
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"cJSON_IsObject":[[2968,9,2968,21,0,0,4,0]],"cJSON_IsString":[[2948,9,2948,21,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_Delete","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_AddFalseToObject","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_CreateString","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_AddFalseToObject","cJSON_Print","cJSON_Delete"],"visited":0}
/*Here is the completed C++ program using cJSON library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from file
    fseek(in_file, 0, SEEK_END);
    size_t length = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *inputData = (char *)malloc(length + 1);
    fread(inputData, 1, length, in_file);
    inputData[length] = '\0';
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_ParseWithOpts
    const char *end = NULL;
    cJSON *root = cJSON_ParseWithOpts(inputData, &end, true);
    if (root == NULL) {
        free(inputData);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the root is an object
    if (!cJSON_IsObject(root)) {
        cJSON_Delete(root);
        free(inputData);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the "format" object from the root
    cJSON *formatObject = cJSON_GetObjectItemCaseSensitive(root, "format");
    if (!cJSON_IsObject(formatObject)) {
        cJSON_Delete(root);
        free(inputData);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the "type" value is a string
    cJSON *typeValue = cJSON_GetObjectItemCaseSensitive(formatObject, "type");
    if (!cJSON_IsString(typeValue)) {
        cJSON_Delete(root);
        free(inputData);
        assert_file_closed(&in_file);
	return 0;
    }

    // Replace the "type" value with a new string
    cJSON *newTypeValue = cJSON_CreateString("circle");
    cJSON_ReplaceItemInArray(formatObject, cJSON_GetArraySize(formatObject) - 1, newTypeValue);

    // Add a new boolean value to the root object
    cJSON_AddFalseToObject(root, "is_valid");

    // Print the modified root object
    char *outputData = cJSON_Print(root);
    free(inputData);

    // Write the output data to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != NULL) {
        fwrite(outputData, 1, strlen(outputData), out_file);
        assert_file_closed(&out_file);;
    }

    free(outputData);
    cJSON_Delete(root);
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}