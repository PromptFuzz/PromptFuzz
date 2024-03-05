#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 427
//<Prompt> ["cJSON_IsBool","cJSON_AddBoolToObject","cJSON_AddNullToObject","cJSON_GetStringValue","cJSON_AddItemToObjectCS","cJSON_malloc","cJSON_CreateArray"]
/*<Combination>: [cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name),
    char *cJSON_GetStringValue(const cJSON *const item),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_CreateArray()
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"cJSON_GetStringValue":[[101,9,101,30,0,0,4,1]]},"library_calls":["cJSON_CreateArray","cJSON_Parse","cJSON_Delete","cJSON_IsString","cJSON_GetStringValue","cJSON_AddItemToArray","cJSON_CreateString","cJSON_Print","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateArray","cJSON_Parse","cJSON_IsString","cJSON_GetStringValue","cJSON_AddItemToArray","cJSON_CreateString","cJSON_Print","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data
    char input[1024];
    if (fgets(input, sizeof(input), in_file) == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Close the input file
    assert_file_closed(&in_file);;

    // Create a cJSON array
    cJSON *array = cJSON_CreateArray();
    if (array == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data as a JSON array
    cJSON *parsedArray = cJSON_Parse(input);
    if (parsedArray == NULL) {
        cJSON_Delete(array);
        assert_file_closed(&in_file);
	return 0;
    }

    // Iterate over the elements in the parsed array
    cJSON *item = parsedArray->child;
    while (item != NULL) {
        // Check if the item is a string
        if (cJSON_IsString(item)) {
            // Get the string value
            char *value = cJSON_GetStringValue(item);

            // Add the string value to the array
            cJSON_AddItemToArray(array, cJSON_CreateString(value));
        }

        // Move to the next item
        item = item->next;
    }

    // Print the array
    char *output = cJSON_Print(array);
    if (output == NULL) {
        cJSON_Delete(parsedArray);
        cJSON_Delete(array);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open the output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        cJSON_Delete(parsedArray);
        cJSON_Delete(array);
        free(output);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the output data
    fputs(output, out_file);

    // Close the output file
    assert_file_closed(&out_file);;

    // Cleanup
    cJSON_Delete(parsedArray);
    cJSON_Delete(array);
    free(output);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}