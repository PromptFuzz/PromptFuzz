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
//<ID> 1512
//<Prompt> ["cJSON_Compare","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetNumberValue","cJSON_AddBoolToObject","cJSON_InsertItemInArray"]
/*<Combination>: [cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    double cJSON_GetNumberValue(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem)
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"cJSON_InsertItemInArray":[[2281,9,2281,39,0,0,4,1]]},"library_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetArrayItem","cJSON_Compare","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetNumberValue","cJSON_AddBoolToObject","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_IsArray","cJSON_GetArrayItem","cJSON_IsObject","cJSON_GetObjectItemCaseSensitive","cJSON_IsString","cJSON_GetArrayItem","cJSON_Compare","cJSON_CreateString","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetNumberValue","cJSON_AddBoolToObject","cJSON_CreateString","cJSON_InsertItemInArray","cJSON_Print","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data from file
    fseek(in_file, 0, SEEK_END);
    long input_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *input_data = new char[input_size + 1];
    fread(input_data, sizeof(char), input_size, in_file);
    input_data[input_size] = '\0';

    // Parse the input data using cJSON
    cJSON *root = cJSON_Parse(input_data);
    if (root == nullptr) {
        // Failed to parse input data
        delete[] input_data;
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform manipulation on the parsed cJSON object
    if (cJSON_IsArray(root)) {
        cJSON *firstItem = cJSON_GetArrayItem(root, fuzz_int32_t_1);
        if (firstItem != nullptr && cJSON_IsObject(firstItem)) {
            cJSON *precision = cJSON_GetObjectItemCaseSensitive(firstItem, fuzz_str_2);
            if (precision != nullptr && cJSON_IsString(precision) && strcmp(precision->valuestring, "zip") == 0) {
                // Compare the first item with the second item
                cJSON *secondItem = cJSON_GetArrayItem(root, fuzz_int32_t_3);
                cJSON_bool result = cJSON_Compare(firstItem, secondItem, cJSON_True);
                
                // Replace the precision value
                cJSON *newPrecision = cJSON_CreateString(fuzz_str_4);
                cJSON_ReplaceItemInObjectCaseSensitive(firstItem, fuzz_str_5, newPrecision);
                
                // Get the value of Latitude
                double latitude = cJSON_GetNumberValue(firstItem);

                // Add a new boolean item to the object
                cJSON_AddBoolToObject(firstItem, fuzz_str_6, cJSON_True);

                // Insert a new item in the array
                cJSON *newItem = cJSON_CreateString(fuzz_str_7);
                cJSON_InsertItemInArray(root, fuzz_int32_t_8, newItem);
            }
        }
    }

    // Print the modified cJSON object
    char *output_data = cJSON_Print(root);

    // Write the output data to file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != NULL) {
        fwrite(output_data, sizeof(char), strlen(output_data), out_file);
        assert_file_closed(&out_file);;
    }

    // Cleanup
    cJSON_Delete(root);
    delete[] input_data;
    free(output_data);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}