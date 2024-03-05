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
//<ID> 285
//<Prompt> ["cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_IsTrue","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"cJSON_CreateBool":[[2423,22,2423,29,0,0,4,1]]},"library_calls":["cJSON_CreateObject","cJSON_CreateObject","cJSON_CreateString","cJSON_CreateString","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_PrintUnformatted","cJSON_Delete","cJSON_free","cJSON_free"],"critical_calls":["cJSON_CreateObject","cJSON_CreateObject","cJSON_CreateString","cJSON_CreateString","cJSON_CreateNumber","cJSON_CreateNumber","cJSON_CreateBool","cJSON_CreateNumber","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_AddItemToObject","cJSON_Print","cJSON_PrintUnformatted","cJSON_Delete","cJSON_free","cJSON_free"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<94) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // Create cJSON objects
    cJSON *root = cJSON_CreateObject();
    cJSON *format = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString(fuzz_str_1);
    cJSON *type = cJSON_CreateString(fuzz_str_2);
    cJSON *width = cJSON_CreateNumber(1920);
    cJSON *height = cJSON_CreateNumber(1080);
    cJSON *interlace = cJSON_CreateBool(false);
    cJSON *frameRate = cJSON_CreateNumber(24);

    // Add items to the format object
    cJSON_AddItemToObject(format, fuzz_str_3, type);
    cJSON_AddItemToObject(format, fuzz_str_4, width);
    cJSON_AddItemToObject(format, fuzz_str_5, height);
    cJSON_AddItemToObject(format, fuzz_str_6, interlace);
    cJSON_AddItemToObject(format, fuzz_str_7, frameRate);

    // Add items to the root object
    cJSON_AddItemToObject(root, fuzz_str_8, name);
    cJSON_AddItemToObject(root, fuzz_str_9, format);

    // Print the cJSON objects
    char *rootStr = cJSON_Print(root);
    char *rootUnformattedStr = cJSON_PrintUnformatted(root);

    // Release resources
    cJSON_Delete(root);
    cJSON_free(rootStr);
    cJSON_free(rootUnformattedStr);

    return 0;
}