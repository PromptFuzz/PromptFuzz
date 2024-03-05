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
//<ID> 1159
//<Prompt> ["cJSON_CreateIntArray","cJSON_ParseWithLengthOpts","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_AddItemReferenceToObject","cJSON_CreateRaw","cJSON_ParseWithOpts","cJSON_ReplaceItemViaPointer"]
/*<Combination>: [cJSON *cJSON_CreateIntArray(const int * numbers, int count),
    cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    void cJSON_DeleteItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_AddItemReferenceToObject(cJSON * object, const char * string, cJSON * item),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    cJSON_bool cJSON_ReplaceItemViaPointer(cJSON *const parent, cJSON *const item, cJSON * replacement)
*/
//<score> 9, nr_unique_branch: 2
//<Quality> {"density":3,"unique_branches":{"cJSON_CreateNumber":[[2442,18,2442,40,0,0,4,0]],"cJSON_CreateIntArray":[[2578,14,2578,22,0,0,4,1]]},"library_calls":["cJSON_CreateIntArray","cJSON_PrintUnformatted","cJSON_Delete"],"critical_calls":["cJSON_CreateIntArray","cJSON_PrintUnformatted","cJSON_Delete"],"visited":0}
/*Please find below the C++ program that utilizes the cJSON library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void*)data, size, "rb");

    // Step 6: Create a string for output file name
    const char* output_file = "output_file";

    // Step 4: Create a FILE * variable for writing output data
    FILE *out_file = fopen(output_file, "wb");

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 3: Consume the input data
    size_t read_size = fread((void*)data, sizeof(uint8_t), size, in_file);

    // Step 2: Use cJSON APIs to achieve the given event
    cJSON *array = cJSON_CreateIntArray((int*)data, read_size / sizeof(int));

    char* array_str = cJSON_PrintUnformatted(array);
    fwrite(array_str, sizeof(char), strlen(array_str), out_file);
    free(array_str);

    cJSON_Delete(array);

    // Step 7: Release all allocated resources
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Return 0 to indicate successful execution
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}