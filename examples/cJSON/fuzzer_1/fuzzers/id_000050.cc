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
//<ID> 1800
//<Prompt> ["cJSON_CreateObject","cJSON_Parse","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_Print","cJSON_PrintUnformatted","cJSON_Delete","cJSON_AddItemToObjectCS","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"]
/*<Combination>: [cJSON *cJSON_CreateObject(),
    cJSON *cJSON_Parse(const char * value),
    cJSON_bool cJSON_IsArray(const cJSON *const item),
    int cJSON_GetArraySize(const cJSON * array),
    cJSON *cJSON_GetArrayItem(const cJSON * array, int index),
    cJSON_bool cJSON_IsString(const cJSON *const item),
    char *cJSON_Print(const cJSON * item),
    char *cJSON_PrintUnformatted(const cJSON * item),
    void cJSON_Delete(cJSON * item),
    cJSON_bool cJSON_AddItemToObjectCS(cJSON * object, const char * string, cJSON * item),
    char *cJSON_PrintUnformatted(const cJSON * item),
    void cJSON_free(void * object),
    void cJSON_Delete(cJSON * item)
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"case_insensitive_strcmp":[[140,9,140,27,0,0,4,0]]},"library_calls":["cJSON_CreateObject","cJSON_Parse","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_Parse","cJSON_AddItemToObjectCS","cJSON_GetObjectItem","cJSON_IsArray","cJSON_GetArraySize","cJSON_GetArrayItem","cJSON_IsString","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create a cJSON object
    cJSON *root = cJSON_CreateObject();
    
    // Step 2: Parse the input data and assign it to the cJSON object
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), in_file) != NULL) {
        cJSON *parsedData = cJSON_Parse(buffer);
        if (parsedData != NULL) {
            cJSON_AddItemToObjectCS(root, fuzz_str_1, parsedData);
        }
    }
    
    // Step 3: Check if "data" is an array and get the array size
    cJSON *dataArray = cJSON_GetObjectItem(root, fuzz_str_2);
    if (cJSON_IsArray(dataArray)) {
        int arraySize = cJSON_GetArraySize(dataArray);
        for (int i = 0; i < arraySize; i++) {
            // Step 3: Get each item in the array
            cJSON *item = cJSON_GetArrayItem(dataArray, fuzz_int32_t_3);

            // Step 6: Print the value of each item if it is a string
            if (cJSON_IsString(item)) {
                char *value = cJSON_PrintUnformatted(item);
                printf("Value: %s\n", value);
                cJSON_free(value);
            }
        }
    }

    // Step 7: Release allocated resources
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (root) {
        cJSON_Delete(root);
    }

    assert_file_closed(&in_file);
	return 0;
}