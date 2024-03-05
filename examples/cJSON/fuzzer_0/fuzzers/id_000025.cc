#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 472
//<Prompt> ["cJSON_Parse","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_CreateRaw","cJSON_PrintBuffered","cJSON_DeleteItemFromObject","cJSON_IsString","cJSON_GetStringValue","cJSON_AddNullToObject","cJSON_CreateArray","cJSON_AddItemToObjectCS","cJSON_CreateNumber","cJSON_AddItemToArray","cJSON_Print","cJSON_Delete"]
/*<Combination>: [cJSON *cJSON_Parse(const char * value),
    cJSON_bool cJSON_IsInvalid(const cJSON *const item),
    cJSON *cJSON_GetObjectItem(const cJSON *const object, const char *const string),
    cJSON *cJSON_CreateRaw(const char * raw),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    void cJSON_DeleteItemFromObject(cJSON * object, const char * string),
    cJSON_bool cJSON_IsString(const cJSON *const item),
    char *cJSON_GetStringValue(const cJSON *const item),
    cJSON *cJSON_AddNullToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_CreateArray(),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    cJSON *cJSON_CreateNumber(double num),
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item),
    char *cJSON_Print(const cJSON * item),
    void cJSON_Delete(cJSON * item)
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_IsInvalid":[[2888,9,2888,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_Delete","cJSON_GetObjectItem","cJSON_IsObject","cJSON_Delete","cJSON_Delete","cJSON_PrintBuffered","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_GetObjectItem","cJSON_IsObject","cJSON_PrintBuffered","cJSON_Delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Parse the input data
    char *input_data = (char *)malloc(size + 1);
    fread(input_data, 1, size, in_file);
    input_data[size] = '\0';
    cJSON *root = cJSON_Parse(input_data);

    // Step 2: Check if the parsing was successful
    if (cJSON_IsInvalid(root)) {
        // Clean up and return
        cJSON_Delete(root);
        free(input_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get object item
    cJSON *glossary = cJSON_GetObjectItem(root, "glossary");
    if (glossary == NULL || !cJSON_IsObject(glossary)) {
        // Clean up and return
        cJSON_Delete(root);
        free(input_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        // Clean up and return
        cJSON_Delete(root);
        free(input_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Write output data
    size_t prebuffer = 0;
    char *output_data = cJSON_PrintBuffered(root, prebuffer, 1);
    fwrite(output_data, 1, strlen(output_data), out_file);

    // Step 7: Clean up resources
    free(output_data);
    free(input_data);
    cJSON_Delete(root);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}