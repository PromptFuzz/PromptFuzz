#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 792
//<Prompt> ["cJSON_CreateObject","cJSON_CreateNumber","cJSON_ReplaceItemInObject","cJSON_ParseWithLength","cJSON_AddRawToObject"]
/*<Combination>: [cJSON *cJSON_CreateObject(),
    cJSON *cJSON_CreateNumber(double num),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw)
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"cJSON_ParseWithLengthOpts":[[1104,9,1104,22,0,0,4,0],[1143,9,1143,21,0,0,4,1],[1148,9,1148,22,0,0,4,1]]},"library_calls":["cJSON_CreateObject","cJSON_ReplaceItemInObject","cJSON_ParseWithLength","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_ReplaceItemInObject","cJSON_ParseWithLength","cJSON_AddRawToObject","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Create a cJSON object
    cJSON *object = cJSON_CreateObject();
    
    // Step 5: Get the file descriptor for reading
    int fd = fuzz_fileno(in_file);
    
    // Step 2: Replace the value of a key in the object
    cJSON_ReplaceItemInObject(object, "Image", cJSON_ParseWithLength(NULL, 0));
    
    // Step 3: Add a raw value to the object
    cJSON_AddRawToObject(object, "Raw", "Raw Data");
    
    // Step 6: Use file name as a string
    const char *file_name = "output_file";
    
    // Step 4: Create a FILE pointer to write the output data
    FILE *out_file = fopen(file_name, "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        cJSON_Delete(object);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 7: Release allocated resources before returning
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cJSON_Delete(object);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}