#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2118
//<Prompt> ["cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_Compare"]
/*<Combination>: [cJSON *cJSON_CreateObjectReference(const cJSON * child),
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring),
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string),
    cJSON *cJSON_CreateTrue(),
    cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive)
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"get_object_item":[[1884,29,1884,43,0,0,4,0]]},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_CreateObjectReference","cJSON_SetValuestring","cJSON_AddStringToObject","cJSON_CreateTrue","cJSON_Compare","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create cJSON object from input data
    char buffer[4096];
    size_t bytesRead = fread(buffer, sizeof(char), sizeof(buffer), in_file);
    cJSON *json = cJSON_ParseWithLengthOpts(buffer, bytesRead, nullptr, cJSON_True);
    assert_file_closed(&in_file);;
    
    if (json != nullptr) {
        // Create a cJSON object reference
        cJSON *reference = cJSON_CreateObjectReference(json);
        
        // Set a valuestring for the cJSON object
        cJSON_SetValuestring(json, "new_valuestring");
        
        // Add a string to the cJSON object
        cJSON_AddStringToObject(json, "new_string", "new_value");
        
        // Create a cJSON true value
        cJSON *cTrue = cJSON_CreateTrue();
        
        // Compare two cJSON objects
        cJSON_Compare(reference, json, cJSON_False);
        
        // Print the cJSON object
        char *jsonStr = cJSON_Print(json);
        printf("%s\n", jsonStr);
        cJSON_free(jsonStr);
        
        // Delete the cJSON objects
        cJSON_Delete(reference);
        cJSON_Delete(json);
        cJSON_Delete(cTrue);
    }
    
    assert_file_closed(&in_file);
	return 0;
}