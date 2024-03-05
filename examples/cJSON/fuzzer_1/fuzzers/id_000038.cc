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
//<ID> 1118
//<Prompt> ["cJSON_CreateIntArray","cJSON_ParseWithLengthOpts","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddItemReferenceToObject","cJSON_CreateRaw","cJSON_ParseWithOpts","cJSON_ReplaceItemViaPointer"]
/*<Combination>: [cJSON *cJSON_CreateIntArray(const int * numbers, int count),
    cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    void cJSON_DeleteItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_AddItemReferenceToObject(cJSON * object, const char * string, cJSON * item),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON_bool cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, cJSON * replacement)
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"create_reference":[[1938,9,1938,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_CreateIntArray","cJSON_AddItemReferenceToObject","cJSON_Delete","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_CreateIntArray","cJSON_AddItemReferenceToObject","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_Delete"],"visited":0}
/*Here is the complete C++ program that achieves the event using the cJSON library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create a cJSON object from the input data
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *json_data = (char *)malloc(file_size + 1);
    fread(json_data, file_size, 1, in_file);
    assert_file_closed(&in_file);;
    json_data[file_size] = '\0';
    cJSON *json_obj = cJSON_Parse(json_data);
    free(json_data);

    if (json_obj == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Call cJSON library APIs as required
    cJSON *array = cJSON_CreateIntArray(nullptr, 0);
    cJSON_AddItemReferenceToObject(json_obj, fuzz_str_1, array);

    // Step 5: Get the file descriptor for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        cJSON_Delete(json_obj);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Step 3: Delete an item from the cJSON object
    cJSON_DeleteItemFromObjectCaseSensitive(json_obj, fuzz_str_2);

    // Step 6: Use the file name as a string directly
    const char *file_name = "output_file";

    // Step 7: Release allocated resources
    assert_file_closed(&out_file);;
    cJSON_Delete(json_obj);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}