#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 330
//<Prompt> ["cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_IsTrue","cJSON_CreateStringArray"]
/*<Combination>: [cJSON *cJSON_CreateBool(cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    cJSON_bool cJSON_IsTrue(const cJSON *const item),
    cJSON *cJSON_CreateStringArray(const char *const * strings, int count)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["cJSON_CreateObject","cJSON_CreateArray","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Delete","cJSON_Print","cJSON_free","cJSON_Delete"],"critical_calls":["cJSON_CreateObject","cJSON_CreateArray","cJSON_CreateBool","cJSON_InsertItemInArray","cJSON_AddItemToObject","cJSON_CreateStringArray","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_Print","cJSON_free","cJSON_Delete"],"visited":0}
/*Here is the code that achieves the event using the cJSON APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 1: Create a cJSON object
    cJSON *root = cJSON_CreateObject();
    
    // Step 2: Insert a boolean value into an array
    cJSON *array = cJSON_CreateArray();
    cJSON *boolean = cJSON_CreateBool(true);
    cJSON_InsertItemInArray(array, 0, boolean);
    cJSON_AddItemToObject(root, "array", array);
    
    // Step 3: Replace an item in the object with a new item
    cJSON *newItem = cJSON_CreateStringArray((const char *[]){"string1", "string2"}, 2);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "array", newItem);
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(stdout);
    FILE *out_file = fdopen(out_fd, "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        cJSON_Delete(root);
        assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }
    
    // Step 6: Print the cJSON object to the output file
    char *jsonString = cJSON_Print(root);
    fprintf(out_file, "%s\n", jsonString);
    
    // Step 7: Free resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cJSON_free(jsonString);
    cJSON_Delete(root);
    
    assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}