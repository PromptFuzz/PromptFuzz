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
//<ID> 1702
//<Prompt> ["cJSON_DeleteItemFromObjectCaseSensitive","cJSON_CreateArray","cJSON_CreateArrayReference","cJSON_GetArraySize","cJSON_AddItemReferenceToArray","cJSON_CreateBool","cJSON_SetNumberHelper","cJSON_Delete"]
/*<Combination>: [void cJSON_DeleteItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON *cJSON_CreateArray(),
    cJSON *cJSON_CreateArrayReference(const cJSON * child),
    int cJSON_GetArraySize(const cJSON * array),
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    cJSON *cJSON_CreateBool(cJSON_bool boolean),
    double cJSON_SetNumberHelper(cJSON * object, double number),
    void cJSON_Delete(cJSON * item)
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"cJSON_PrintPreallocated":[[1296,25,1296,41,0,0,4,0]]},"library_calls":["cJSON_CreateArray","cJSON_CreateBool","cJSON_CreateNumber","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_SetNumberHelper","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_CreateArray","cJSON_CreateBool","cJSON_CreateNumber","cJSON_CreateObject","cJSON_CreateString","cJSON_AddItemReferenceToArray","cJSON_AddItemToObject","cJSON_SetNumberHelper","cJSON_DeleteItemFromObjectCaseSensitive","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_PrintPreallocated","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Create a FILE stream to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create cJSON items
    cJSON *array = cJSON_CreateArray();
    cJSON *bool_item = cJSON_CreateBool(true);
    cJSON *number_item = cJSON_CreateNumber(42.0);
    cJSON *object = cJSON_CreateObject();
    cJSON *object_item = cJSON_CreateString(fuzz_str_1);

    // Add items to array and object
    cJSON_AddItemReferenceToArray(array, bool_item);
    cJSON_AddItemToObject(object, fuzz_str_2, object_item);

    // Set number value of an item
    cJSON_SetNumberHelper(number_item, 3.14);

    // Delete an item from object
    cJSON_DeleteItemFromObjectCaseSensitive(object, fuzz_str_3);

    // Write cJSON items to a FILE stream
    cJSON_PrintPreallocated(array, nullptr, 0, true);
    cJSON_PrintPreallocated(bool_item, nullptr, 0, true);
    cJSON_PrintPreallocated(number_item, nullptr, 0, true);
    cJSON_PrintPreallocated(object, nullptr, 0, true);

    // Delete cJSON items
    cJSON_Delete(array);
    cJSON_Delete(bool_item);
    cJSON_Delete(number_item);
    cJSON_Delete(object);

    // Close the FILE stream
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}