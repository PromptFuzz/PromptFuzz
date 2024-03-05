#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1369
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
//<score> 34, nr_unique_branch: 8
//<Quality> {"density":17,"unique_branches":{"skip_multiline_comment":[[2806,12,2806,31,0,0,4,0],[2806,12,2806,31,0,0,4,1],[2808,13,2808,33,0,0,4,0],[2808,13,2808,33,0,0,4,1],[2808,37,2808,57,0,0,4,0],[2808,37,2808,57,0,0,4,1]],"skip_oneline_comment":[[2795,13,2795,32,0,0,4,0]],"cJSON_Minify":[[2863,26,2863,40,0,0,4,0]]},"library_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_AddBoolToObject","cJSON_Print","cJSON_Minify","cJSON_IsFalse","cJSON_DeleteItemFromArray","cJSON_CreateArrayReference","cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_CreateFalse","cJSON_Duplicate","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_malloc","cJSON_Parse","cJSON_free","cJSON_AddBoolToObject","cJSON_Print","cJSON_Minify","cJSON_IsFalse","cJSON_DeleteItemFromArray","cJSON_CreateArrayReference","cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_CreateFalse","cJSON_Duplicate","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_free"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file stream to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a cJSON object from the input data
    fseek(in_file, 0, SEEK_END);
    long fsize = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *buffer = (char *)cJSON_malloc(fsize + 1);
    fread(buffer, fsize, 1, in_file);
    assert_file_closed(&in_file);;
    
    buffer[fsize] = '\0';
    cJSON *root = cJSON_Parse(buffer);
    cJSON_free(buffer);
    
    if (root == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Add a boolean value to the cJSON object
    cJSON_AddBoolToObject(root, "isNew", 1);
    
    // Minify the cJSON object
    char *minified = cJSON_Print(root);
    cJSON_Minify(minified);
    
    // Check if the cJSON object is false
    if (cJSON_IsFalse(root)) {
        cJSON_DeleteItemFromArray(root, 0);
    }
    
    // Create a cJSON array reference
    cJSON *arrayRef = cJSON_CreateArrayReference(root);
    
    // Create a cJSON object reference
    cJSON *objectRef = cJSON_CreateObjectReference(root);
    
    // Set a valuestring for the cJSON object
    cJSON_SetValuestring(root, "status");
    
    // Create a cJSON false object
    cJSON *falseObj = cJSON_CreateFalse();
    
    // Duplicate the cJSON object
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    
    // Clean up resources
    cJSON_Delete(duplicate);
    cJSON_Delete(falseObj);
    cJSON_Delete(objectRef);
    cJSON_Delete(arrayRef);
    cJSON_Delete(root);
    cJSON_free(minified);
    
    assert_file_closed(&in_file);
	return 0;
}