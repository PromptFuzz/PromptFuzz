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
//<ID> 45
//<Prompt> ["cJSON_AddItemReferenceToArray","cJSON_Version","cJSON_CreateTrue","cJSON_CreateDoubleArray","cJSON_AddRawToObject","cJSON_CreateIntArray","cJSON_AddFalseToObject","cJSON_SetValuestring"]
/*<Combination>: [cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    const char *cJSON_Version(),
    cJSON *cJSON_CreateTrue(),
    cJSON *cJSON_CreateDoubleArray(const double * numbers, int count),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw),
    cJSON *cJSON_CreateIntArray(const int * numbers, int count),
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name),
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring)
*/
//<score> 12, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"cJSON_CreateDoubleArray":[[2632,24,2632,41,0,0,4,1],[2639,16,2639,17,0,0,4,0],[2639,21,2639,40,0,0,4,0],[2639,21,2639,40,0,0,4,1],[2642,12,2642,14,0,0,4,1],[2647,12,2647,14,0,0,4,0],[2647,12,2647,14,0,0,4,1],[2658,9,2658,10,0,0,4,0],[2658,14,2658,22,0,0,4,0]]},"library_calls":["cJSON_ParseWithLength","cJSON_AddTrueToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_SetValuestring","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_AddTrueToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_AddFalseToObject","cJSON_SetValuestring","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<130) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeDoublingArray(fuzz_double_array_2, fuzz_double_array_size_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegralArray(int32_t, fuzz_int32_t_array_6, fuzz_int32_t_array_size_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	
		const double * fuzzer_var_cJSON_CreateDoubleArray_2_0 = fuzz_double_array_2;
		int fuzzer_size_cJSON_CreateDoubleArray_2_0 = static_cast<int>(fuzz_double_array_size_2);
		const int * fuzzer_var_cJSON_CreateIntArray_5_0 = fuzz_int32_t_array_6;
		int fuzzer_size_cJSON_CreateIntArray_5_0 = static_cast<int>(fuzz_int32_t_array_size_6);//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data using cJSON_ParseWithLength
    char buffer[4096];
    size_t n = fread(buffer, sizeof(char), sizeof(buffer), in_file);
    cJSON *root = cJSON_ParseWithLength(buffer, n);
    assert_file_closed(&in_file);;

    if (root == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Add a true value to an object
    cJSON_AddTrueToObject(root, fuzz_str_1);

    // Create an array of doubles
    double numbers[] = {1.23, 4.56, 7.89};
    cJSON *doubleArray = cJSON_CreateDoubleArray(fuzzer_var_cJSON_CreateDoubleArray_2_0, fuzzer_size_cJSON_CreateDoubleArray_2_0);
    cJSON_AddItemToObject(root, fuzz_str_3, doubleArray);

    // Add a raw value to an object
    const char *rawValue = "{\"key\": \"value\"}";
    cJSON_AddRawToObject(root, fuzz_str_4, fuzz_str_5);

    // Create an array of integers
    int intNumbers[] = {1, 2, 3};
    cJSON *intArray = cJSON_CreateIntArray(fuzzer_var_cJSON_CreateIntArray_5_0, fuzzer_size_cJSON_CreateIntArray_5_0);
    cJSON_AddItemToObject(root, fuzz_str_7, intArray);

    // Add a false value to an object
    cJSON_AddFalseToObject(root, fuzz_str_8);

    // Set a value string
    cJSON_SetValuestring(root, fuzz_str_9);

    // Print the modified cJSON object
    char *output = cJSON_Print(root);
    printf("%s\n", output);
    cJSON_free(output);

    // Delete the cJSON object
    cJSON_Delete(root);

    assert_file_closed(&in_file);
	return 0;
}