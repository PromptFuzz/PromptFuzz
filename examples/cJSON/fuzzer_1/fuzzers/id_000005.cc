#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 81
//<Prompt> ["cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_DetachItemFromArray","cJSON_PrintPreallocated"]
/*<Combination>: [cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_IsNull(const cJSON *const item),
    cJSON_bool cJSON_IsArray(const cJSON *const item),
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string),
    cJSON *cJSON_DetachItemFromObject(cJSON * object, const char * string),
    cJSON *cJSON_DetachItemFromArray(cJSON * array, int which),
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_IsArray","cJSON_IsNumber","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_IsArray","cJSON_IsNumber","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Parse JSON input
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    char *json_str = new char[file_size + 1];
    fread(json_str, file_size, 1, in_file);
    json_str[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_str);
    delete[] json_str;
    assert_file_closed(&in_file);;

    if (root == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Manipulate JSON object
    if (cJSON_IsArray(root)) {
        cJSON *array = root->child;
        while (array != NULL) {
            if (cJSON_IsArray(array)) {
                cJSON *subArray = array->child;
                while (subArray != NULL) {
                    if (cJSON_IsNumber(subArray)) {
                        // Increment the value by 1
                        subArray->valuedouble += 1;
                        subArray->valueint += 1;
                    }
                    subArray = subArray->next;
                }
            }
            array = array->next;
        }
    }

    // Step 3: Convert JSON object back to string
    char *output_str = cJSON_Print(root);

    // Step 4: Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        cJSON_Delete(root);
        free(output_str);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Write output to file
    fwrite(output_str, strlen(output_str), 1, out_file);
    assert_file_closed(&out_file);;

    // Step 7: Release resources
    cJSON_Delete(root);
    free(output_str);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}