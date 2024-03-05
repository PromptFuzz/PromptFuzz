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
//<ID> 935
//<Prompt> ["cJSON_IsRaw","cJSON_AddItemReferenceToArray","cJSON_CreateFalse","cJSON_IsFalse","cJSON_CreateStringReference"]
/*<Combination>: [cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_CreateFalse(),
    cJSON_bool cJSON_IsFalse(const cJSON *const item),
    cJSON *cJSON_CreateStringReference(const char * string)
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"cJSON_HasObjectItem":[[1924,12,1924,47,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateFalse","cJSON_AddItemReferenceToArray","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_HasObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_IsString","cJSON_CreateFalse","cJSON_AddItemReferenceToArray","cJSON_Print","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the input data using cJSON_Parse
    char *inputData = nullptr;
    size_t bytesRead = 0;
    char buffer[1024];
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        inputData = (char *)realloc(inputData, size + bytesRead + 1);
        memcpy(inputData + size, buffer, bytesRead);
        size += bytesRead;
    }
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_ParseWithLength
    cJSON *root = cJSON_ParseWithLength(inputData, size);
    free(inputData);

    if (!root) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the root object has a "widget" key
    if (cJSON_HasObjectItem(root, fuzz_str_1)) {
        // Get the "widget" object
        cJSON *widget = cJSON_GetObjectItem(root, fuzz_str_2);

        // Check if the "widget" object has a "debug" key with value "on"
        cJSON *debug = cJSON_GetObjectItem(widget, fuzz_str_3);
        if (debug && cJSON_IsString(debug) && strcmp(debug->valuestring, "on") == 0) {
            // Create a false boolean value
            cJSON *falseValue = cJSON_CreateFalse();
            // Add the false value to an array in the "widget" object
            cJSON_AddItemReferenceToArray(widget, falseValue);
        }
    }

    // Print the modified cJSON structure
    char *outputData = cJSON_Print(root);
    cJSON_Delete(root);

    // Create a FILE pointer to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the output data to the FILE pointer
    fwrite(outputData, strlen(outputData), 1, out_file);
    assert_file_closed(&out_file);;

    free(outputData);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}