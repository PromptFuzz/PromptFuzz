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
//<ID> 10
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
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["cJSON_Version","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_CreateString","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_Version","cJSON_CreateDoubleArray","cJSON_CreateObject","cJSON_CreateString","cJSON_AddRawToObject","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Get cJSON version
    const char *version = cJSON_Version();
    printf("cJSON version: %s\n", version);

    // Step 2: Create cJSON objects using various APIs
    cJSON *array = cJSON_CreateDoubleArray(NULL, 0);
    cJSON *object = cJSON_CreateObject();
    cJSON *item = cJSON_CreateString(fuzz_str_1);

    // Step 3: Consume input data using cJSON APIs
    char buffer[4096];
    fread(buffer, 1, sizeof(buffer), in_file);
    cJSON_AddRawToObject(object, fuzz_str_2, buffer);

    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    printf("Input file descriptor: %d\n", in_fd);

    // Step 6: Use file name as string
    const char *filename = "output_file";

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    cJSON_Delete(array);
    cJSON_Delete(object);
    cJSON_Delete(item);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}