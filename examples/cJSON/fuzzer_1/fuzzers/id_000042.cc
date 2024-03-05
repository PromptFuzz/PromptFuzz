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
//<ID> 1317
//<Prompt> ["cJSON_AddBoolToObject","cJSON_Minify","cJSON_IsFalse","cJSON_DeleteItemFromArray","cJSON_CreateArrayReference","cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_CreateFalse","cJSON_Duplicate"]
/*<Combination>: [cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    void cJSON_Minify(char * json),
    cJSON_bool cJSON_IsFalse(const cJSON *const item),
    void cJSON_DeleteItemFromArray(cJSON * array, int which),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    cJSON *cJSON_CreateObjectReference(const cJSON * child),
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring),
    cJSON *cJSON_CreateFalse(),
    cJSON *cJSON_Duplicate(const cJSON * item, cJSON_bool recurse)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_Print","cJSON_Minify","cJSON_IsFalse","cJSON_CreateFalse","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_AddBoolToObject","cJSON_Print","cJSON_Minify","cJSON_CreateFalse","cJSON_IsFalse","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable to read the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Get the file name
    const char* in_file_name = "input_file";

    // Step 1: Add a boolean value to an object
    cJSON* object = cJSON_CreateObject();
    cJSON_AddBoolToObject(object, fuzz_str_1, true);

    // Step 2: Minify the JSON data
    char* json_data = cJSON_Print(object);
    cJSON_Minify(json_data);

    // Step 3: Check if a cJSON item is false
    cJSON* item = cJSON_IsFalse(object) ? object : cJSON_CreateFalse();

    // Step 7: Release allocated resources
    cJSON_Delete(item);
    cJSON_Delete(object);
    free(json_data);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}