#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 255
//<Prompt> ["cJSON_ParseWithLength","cJSON_CreateObjectReference","cJSON_AddObjectToObject","cJSON_Minify","cJSON_AddNumberToObject","cJSON_AddItemReferenceToArray"]
/*<Combination>: [cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON *cJSON_CreateObjectReference(const cJSON * child),
    cJSON *cJSON_AddObjectToObject(cJSON *const object, const char *const name),
    void cJSON_Minify(char * json),
    cJSON *cJSON_AddNumberToObject(cJSON *const object, const char *const name, const double number),
    cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item)
*/
//<score> 3.5, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["cJSON_ParseWithLength","cJSON_AddNumberToObject","cJSON_CreateObjectReference","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_AddNumberToObject","cJSON_CreateObjectReference","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read the input data from the file pointer and parse it with cJSON
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *json_data = new char[file_size + 1];
    fread(json_data, file_size, 1, in_file);
    assert_file_closed(&in_file);;
    
    cJSON *root = cJSON_ParseWithLength(json_data, file_size);
    delete[] json_data;
    
    if (root == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Add a number value to the parsed cJSON object
    cJSON_AddNumberToObject(root, "New Number", 123.456);
    
    // Create a reference to the cJSON object
    cJSON *object_ref = cJSON_CreateObjectReference(root);
    
    // Create a new cJSON object and add the reference to it
    cJSON *new_object = cJSON_CreateObject();
    cJSON_AddObjectToObject(new_object, "Referenced Object")->child = object_ref;
    
    // Minify the cJSON object into a string
    char *minified_json = cJSON_PrintUnformatted(root);
    cJSON_Minify(minified_json);
    
    // Create a FILE pointer for writing the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        cJSON_Delete(root);
        cJSON_Delete(new_object);
        delete[] minified_json;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write the minified cJSON object to the output file
    fwrite(minified_json, strlen(minified_json), 1, out_file);
    assert_file_closed(&out_file);;
    
    // Cleanup
    cJSON_Delete(root);
    cJSON_Delete(new_object);
    delete[] minified_json;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}