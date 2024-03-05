#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1846
//<Prompt> ["cJSON_Parse","cJSON_ReplaceItemInArray","cJSON_GetArraySize","cJSON_ReplaceItemViaPointer","cJSON_IsNumber","cJSON_GetNumberValue","cJSON_AddFalseToObject","cJSON_SetNumberHelper","cJSON_GetObjectItem","cJSON_IsInvalid","cJSON_PrintUnformatted","cJSON_Delete"]
/*<Combination>: [cJSON *cJSON_Parse(const char * value),
    cJSON_bool cJSON_ReplaceItemInArray(cJSON * array, int which, cJSON * newitem),
    int cJSON_GetArraySize(const cJSON * array),
    cJSON_bool cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, cJSON * replacement),
    cJSON_bool cJSON_IsNumber(const cJSON *const item),
    double cJSON_GetNumberValue(const cJSON *const item),
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name),
    double cJSON_SetNumberHelper(cJSON * object, double number),
    cJSON *cJSON_GetObjectItem(const cJSON *const object, const char *const string),
    cJSON_bool cJSON_IsInvalid(const cJSON *const item),
    char *cJSON_PrintUnformatted(const cJSON * item),
    void cJSON_Delete(cJSON * item)
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"cJSON_GetArraySize":[[1833,9,1833,22,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_PrintUnformatted","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_CreateNumber","cJSON_ReplaceItemViaPointer","cJSON_PrintUnformatted","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Open input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Parse the input JSON data
    char *json_data = nullptr;
    size_t json_size = 0;
    char *line = nullptr;
    size_t line_length = 0;
    while (getline(&line, &line_length, in_file) != -1) {
        json_data = (char *) realloc(json_data, json_size + line_length);
        memcpy(json_data + json_size, line, line_length);
        json_size += line_length;
    }

    cJSON *root = cJSON_Parse(json_data);
    free(json_data);
    free(line);

    if (root == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Modify the parsed JSON data
    cJSON *menu = cJSON_GetObjectItem(root, "menu");
    cJSON *items = cJSON_GetObjectItem(menu, "items");
    int array_size = cJSON_GetArraySize(items);
    if (array_size > 0) {
        // Replace the first item's id with a number
        cJSON *first_item = cJSON_GetArrayItem(items, 0);
        cJSON *id_value = cJSON_CreateNumber(123);
        cJSON_ReplaceItemViaPointer(items, first_item, id_value);
    }

    // Step 3: Print the modified JSON data
    char *modified_json = cJSON_PrintUnformatted(root);
    std::cout << "Modified JSON: " << modified_json << std::endl;
    free(modified_json);

    // Step 5: Open output file stream
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Get file descriptors and file names
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    std::string in_file_name = "input_file";
    std::string out_file_name = "output_file";

    // Step 7: Cleanup
    cJSON_Delete(root);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}