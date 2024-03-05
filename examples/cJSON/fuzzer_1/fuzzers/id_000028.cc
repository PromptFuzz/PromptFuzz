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
//<ID> 529
//<Prompt> ["cJSON_CreateFloatArray","cJSON_CreateArrayReference","cJSON_AddItemToArray","cJSON_AddRawToObject","cJSON_malloc","cJSON_Duplicate","cJSON_AddItemToObject","cJSON_HasObjectItem"]
/*<Combination>: [cJSON *cJSON_CreateFloatArray(const float * numbers, int count),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw),
    void *cJSON_malloc(size_t size),
    cJSON *cJSON_Duplicate(const cJSON * item, cJSON_bool recurse),
    cJSON_bool cJSON_AddItemToObject(cJSON * object, const char * string, cJSON * item),
    cJSON_bool cJSON_HasObjectItem(const cJSON * object, const char * string)
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["cJSON_CreateArray","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToArray","cJSON_HasObjectItem","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_CreateArray","cJSON_CreateFloatArray","cJSON_AddItemToArray","cJSON_CreateObject","cJSON_AddItemToObject","cJSON_CreateString","cJSON_AddItemToArray","cJSON_HasObjectItem","cJSON_Duplicate","cJSON_AddItemToArray","cJSON_PrintUnformatted","cJSON_free","cJSON_Delete"],"visited":1}
/*Based on the provided instructions, here is the C++ program that utilizes the cJSON library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeFloatingArray(fuzz_float_array_1, fuzz_float_array_size_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	
		const float * fuzzer_var_cJSON_CreateFloatArray_1_0 = fuzz_float_array_1;
		int fuzzer_size_cJSON_CreateFloatArray_1_0 = static_cast<int>(fuzz_float_array_size_1);//fuzzer shim end}




    // Step 1: Create cJSON objects and add them to an array
    cJSON *array = cJSON_CreateArray();
    
    float numbers[] = {1.0f, 2.0f, 3.0f};
    cJSON *floatArray = cJSON_CreateFloatArray(fuzzer_var_cJSON_CreateFloatArray_1_0, fuzzer_size_cJSON_CreateFloatArray_1_0);
    cJSON_AddItemToArray(array, floatArray);
    
    cJSON *object = cJSON_CreateObject();
    cJSON_AddItemToObject(object, fuzz_str_2, cJSON_CreateString(fuzz_str_3));
    cJSON_AddItemToArray(array, object);
    
    cJSON_bool hasObject = cJSON_HasObjectItem(object, fuzz_str_4);
    if (hasObject) {
        cJSON *duplicateObject = cJSON_Duplicate(object, true);
        cJSON_AddItemToArray(array, duplicateObject);
    }
    
    // Step 2: Print the cJSON array to a raw string
    char *output = cJSON_PrintUnformatted(array);
    printf("%s\n", output);
    cJSON_free(output);
    
    // Step 3: Release allocated resources
    cJSON_Delete(array);
    
    return 0;
}