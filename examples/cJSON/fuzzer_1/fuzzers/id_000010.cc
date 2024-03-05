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
//<ID> 128
//<Prompt> ["cJSON_ParseWithLengthOpts","cJSON_PrintBuffered","cJSON_CreateRaw","cJSON_IsRaw","cJSON_ReplaceItemInObject","cJSON_DeleteItemFromObject"]
/*<Combination>: [cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    void cJSON_DeleteItemFromObject(cJSON * object, const char * string)
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_PrintBuffered","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_PrintBuffered","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Create a FILE * to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input data from the file
    char *input_data = (char *)malloc(size + 1);
    if (input_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    fread(input_data, 1, size, in_file);
    input_data[size] = '\0';

    // Parse the input data as a cJSON object
    cJSON *root = cJSON_Parse(input_data);
    if (root == NULL) {
        free(input_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Generate output data by printing the cJSON object
    char *output_data = cJSON_PrintBuffered(root, fuzz_int32_t_1, fuzz_int32_t_2);

    // Create a FILE * to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        free(input_data);
        assert_file_closed(&in_file);;
        cJSON_Delete(root);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the output data to the file
    fwrite(output_data, 1, strlen(output_data), out_file);

    // Release resources
    free(input_data);
    assert_file_closed(&in_file);;
    cJSON_Delete(root);
    free(output_data);
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}