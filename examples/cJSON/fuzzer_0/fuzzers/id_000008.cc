#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 103
//<Prompt> ["cJSON_ParseWithLengthOpts","cJSON_PrintBuffered","cJSON_CreateRaw","cJSON_IsRaw","cJSON_ReplaceItemInObject","cJSON_DeleteItemFromObject"]
/*<Combination>: [cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    void cJSON_DeleteItemFromObject(cJSON * object, const char * string)
*/
//<score> 4.3333335, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_ReplaceItemInObject","cJSON_CreateRaw","cJSON_PrintBuffered","cJSON_DeleteItemFromObject","cJSON_DeleteItemFromObject","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsString","cJSON_ReplaceItemInObject","cJSON_CreateRaw","cJSON_PrintBuffered","cJSON_DeleteItemFromObject","cJSON_DeleteItemFromObject","cJSON_Delete"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    cJSON* root = nullptr;
    cJSON* item = nullptr;

    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    char* input_string = nullptr;
    size_t input_length = 0;
    fseek(in_file, 0, SEEK_END);
    input_length = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    input_string = (char*)malloc(input_length + 1);
    fread(input_string, 1, input_length, in_file);
    input_string[input_length] = '\0';

    root = cJSON_Parse(input_string);
    if (root == nullptr) {
        free(input_string);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Use cJSON APIs to manipulate the parsed JSON data
    cJSON* widget = cJSON_GetObjectItem(root, "widget");
    if (widget != nullptr && cJSON_IsObject(widget)) {
        cJSON* text = cJSON_GetObjectItem(widget, "text");
        if (text != nullptr && cJSON_IsObject(text)) {
            cJSON* onMouseUp = cJSON_GetObjectItem(text, "onMouseUp");
            if (onMouseUp != nullptr && cJSON_IsString(onMouseUp)) {
                cJSON_ReplaceItemInObject(text, "onMouseUp", cJSON_CreateRaw("new value"));
            }
        }
    }

    // Print the manipulated JSON data
    char* output_string = cJSON_PrintBuffered(root, 256, 0);
    if (output_string != nullptr) {
        printf("%s\n", output_string);
        free(output_string);
    }

    cJSON_DeleteItemFromObject(root, "widget");
    cJSON_DeleteItemFromObject(root, "image");

    cJSON_Delete(root);
    free(input_string);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}