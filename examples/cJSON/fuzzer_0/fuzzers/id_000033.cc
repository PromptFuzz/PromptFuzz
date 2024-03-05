#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 807
//<Prompt> ["cJSON_Compare","cJSON_AddObjectToObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_SetValuestring"]
/*<Combination>: [cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive),
    cJSON *cJSON_AddObjectToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_AddTrueToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_CreateStringReference(const char * string),
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring)
*/
//<score> 22, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"cJSON_Compare":[[2988,9,2988,20,0,0,4,0],[3097,21,3097,73,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_Compare","cJSON_AddTrueToObject","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_Compare","cJSON_AddTrueToObject","cJSON_Print","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE handle to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from the file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *input_data = (char *)malloc(input_size + 1);
    if (input_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(input_data, 1, input_size, in_file);
    input_data[input_size] = '\0';  // Null-terminate the input data

    // Close the input file handle
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_Parse
    cJSON *root = cJSON_Parse(input_data);
    if (root == NULL) {
        free(input_data);
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform the event using cJSON APIs
    cJSON *widgetObj = cJSON_GetObjectItem(root, "widget");
    if (widgetObj != NULL && cJSON_IsObject(widgetObj)) {
        cJSON *debugObj = cJSON_GetObjectItem(widgetObj, "debug");
        if (debugObj != NULL && cJSON_IsString(debugObj)) {
            if (strcmp(debugObj->valuestring, "on") == 0) {
                // Compare two cJSON objects
                cJSON *windowObj = cJSON_GetObjectItem(widgetObj, "window");
                cJSON *textObj = cJSON_GetObjectItem(widgetObj, "text");
                cJSON_bool isSame = cJSON_Compare(windowObj, textObj, 1);
                if (isSame) {
                    // Add a true value to the object
                    cJSON_AddTrueToObject(widgetObj, "isSame");
                }
            }
        }
    }

    // Convert the modified cJSON structure back to a string
    char *output_data = cJSON_Print(root);

    // Print the output data
    printf("%s\n", output_data);

    // Free the allocated resources
    free(input_data);
    free(output_data);
    cJSON_Delete(root);

    assert_file_closed(&in_file);
	return 0;
}