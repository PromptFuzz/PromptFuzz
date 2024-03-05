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
//<ID> 473
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
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"cJSON_GetStringValue":[[101,9,101,30,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_Delete","cJSON_GetStringValue","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_GetStringValue","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsInvalid","cJSON_GetStringValue","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsObject","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_GetStringValue","cJSON_Print","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<77) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Step 4: Read input data from FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Parse JSON data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    if (json_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    fread(json_data, file_size, 1, in_file);
    assert_file_closed(&in_file);;
    json_data[file_size] = '\0';

    cJSON *root = cJSON_Parse(json_data);
    free(json_data);

    if (root == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Use cJSON APIs on the parsed data
    if (cJSON_IsInvalid(root)) {
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	return 0;
    }

    char *valuestring = cJSON_GetStringValue(root);
    if (valuestring != NULL) {
        printf("Value: %s\n", valuestring);
    }

    cJSON *glossary = cJSON_GetObjectItem(root, fuzz_str_1);
    if (glossary != NULL && cJSON_IsObject(glossary)) {
        cJSON *glossDiv = cJSON_GetObjectItem(glossary, fuzz_str_2);
        if (glossDiv != NULL && cJSON_IsObject(glossDiv)) {
            cJSON *glossList = cJSON_GetObjectItem(glossDiv, fuzz_str_3);
            if (glossList != NULL && cJSON_IsObject(glossList)) {
                cJSON *glossEntry = cJSON_GetObjectItem(glossList, fuzz_str_4);
                if (glossEntry != NULL && cJSON_IsObject(glossEntry)) {
                    cJSON *glossDef = cJSON_GetObjectItem(glossEntry, fuzz_str_5);
                    if (glossDef != NULL && cJSON_IsObject(glossDef)) {
                        cJSON *glossSeeAlso = cJSON_GetObjectItem(glossDef, fuzz_str_6);
                        if (glossSeeAlso != NULL && cJSON_IsArray(glossSeeAlso)) {
                            int arraySize = cJSON_GetArraySize(glossSeeAlso);
                            for (int i = 0; i < arraySize; i++) {
                                cJSON *item = cJSON_GetArrayItem(glossSeeAlso, fuzz_int32_t_7);
                                if (item != NULL && cJSON_IsString(item)) {
                                    printf("GlossSeeAlso: %s\n", cJSON_GetStringValue(item));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Step 3: Print cJSON data and write to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        cJSON_Delete(root);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    char *json_string = cJSON_Print(root);
    if (json_string != NULL) {
        fputs(json_string, out_file);
        free(json_string);
    }

    assert_file_closed(&out_file);;
    cJSON_Delete(root);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}