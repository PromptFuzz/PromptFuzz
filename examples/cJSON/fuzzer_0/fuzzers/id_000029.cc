#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 629
//<Prompt> ["cJSON_ParseWithOpts","cJSON_IsBool","cJSON_IsRaw","cJSON_IsNumber","cJSON_ReplaceItemInArray","cJSON_CreateArrayReference","cJSON_AddFalseToObject"]
/*<Combination>: [cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_IsNumber(const cJSON *const item),
    cJSON_bool cJSON_ReplaceItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name)
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"cJSON_ReplaceItemInArray":[[2343,9,2343,18,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_ReplaceItemInArray","cJSON_CreateString","cJSON_AddFalseToObject","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // 4. Create a FILE* variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Create a string for the output file name
    const char *output_file = "output_file";

    // Parse the input JSON string
    fseek(in_file, 0, SEEK_END);
    long length = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *buffer = (char *)malloc(length + 1);
    fread(buffer, 1, length, in_file);
    buffer[length] = '\0';
    cJSON *root = cJSON_Parse(buffer);
    free(buffer);

    if (!root) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Check if the parsed JSON is valid and contains the necessary objects
    cJSON *widget = cJSON_GetObjectItem(root, "widget");
    if (!widget || !cJSON_IsObject(widget)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Modify the parsed JSON by replacing a specific item in the array
    cJSON *text = cJSON_GetObjectItem(widget, "text");
    if (text && cJSON_IsObject(text)) {
        cJSON *alignment = cJSON_GetObjectItem(text, "alignment");
        if (alignment && cJSON_IsString(alignment)) {
            cJSON_ReplaceItemInArray(text, 0, cJSON_CreateString("right"));
        }
    }

    // Add a new boolean value to the widget object
    cJSON_AddFalseToObject(widget, "newBoolean");

    // 3. Write the modified JSON to the output file
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    char *printed = cJSON_Print(root);
    if (!printed) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fputs(printed, out_file);
    free(printed);

    // 7. Release allocated resources
    cJSON_Delete(root);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}