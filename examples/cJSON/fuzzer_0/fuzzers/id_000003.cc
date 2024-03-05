#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 59
//<Prompt> ["cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_DetachItemFromArray","cJSON_PrintPreallocated"]
/*<Combination>: [cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_IsNull(const cJSON *const item),
    cJSON_bool cJSON_IsArray(const cJSON *const item),
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string),
    cJSON *cJSON_DetachItemFromObject(cJSON * object, const char * string),
    cJSON *cJSON_DetachItemFromArray(cJSON * array, int which),
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format)
*/
//<score> 22, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"cJSON_IsNull":[[2928,9,2928,21,0,0,4,0],[2928,9,2928,21,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_AddStringToObject","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_DetachItemFromObjectCaseSensitive","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItemCaseSensitive","cJSON_IsNull","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file stream to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file stream
    char input[4096]; // Assuming the input data is no larger than 4096 bytes
    size_t bytes_read = fread(input, 1, sizeof(input), in_file);
    assert_file_closed(&in_file);;

    // Create a cJSON object and parse the input data
    cJSON *root = cJSON_Parse(input);
    if (!root) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform operations on the cJSON object
    cJSON *format = cJSON_GetObjectItemCaseSensitive(root, "format");
    if (cJSON_IsNull(format)) {
        // If "format" is null, add a new string to the root object
        cJSON_AddStringToObject(root, "format", "new_format");
    } else if (cJSON_IsArray(format)) {
        // If "format" is an array, detach an item from the array
        cJSON *item = cJSON_DetachItemFromArray(format, 0);
        cJSON_Delete(item);
    } else {
        // If "format" is an object, detach an item from the object
        cJSON *item = cJSON_DetachItemFromObjectCaseSensitive(root, "format");
        cJSON_Delete(item);
    }

    // Print the cJSON object to a buffer
    char output[4096]; // Assuming the output data is no larger than 4096 bytes
    cJSON_PrintPreallocated(root, output, sizeof(output), 1);

    // Write the output data to a file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(output, 1, strlen(output), out_file);
        assert_file_closed(&out_file);;
    }

    // Release the cJSON object
    cJSON_Delete(root);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}