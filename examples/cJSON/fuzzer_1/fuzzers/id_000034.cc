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
//<ID> 818
//<Prompt> ["cJSON_Compare","cJSON_AddObjectToObject","cJSON_AddTrueToObject","cJSON_CreateStringReference","cJSON_SetValuestring"]
/*<Combination>: [cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive),
    cJSON *cJSON_AddObjectToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_AddTrueToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_CreateStringReference(const char * string),
    char *cJSON_SetValuestring(cJSON * object, const char * valuestring)
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":4,"unique_branches":{"cJSON_Compare":[[3011,9,3011,15,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"critical_calls":["cJSON_Parse","cJSON_Print","cJSON_free","cJSON_Delete","cJSON_free"],"visited":0}
/**/


// Function to compare two cJSON objects
cJSON_bool  id_000034_compareCJSONObjects(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive) {
    return cJSON_Compare(a, b, case_sensitive);
}

// Function to create and add a new object to the given cJSON object
cJSON * id_000034_createAndAddObject(cJSON *const object, const char *const name) {
    return cJSON_AddObjectToObject(object, name);
}

// Function to create and add a new boolean value "true" to the given cJSON object
cJSON * id_000034_createAndAddTrueValue(cJSON *const object, const char *const name) {
    return cJSON_AddTrueToObject(object, name);
}

// Function to create a string reference and set the value of the given cJSON object
char * id_000034_createStringReferenceAndSetValue(cJSON *object, const char *valuestring) {
    return cJSON_SetValuestring(object, valuestring);
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a file descriptor for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Parse the input data using cJSON_Parse
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *json_data = new char[file_size + 1];
    fread(json_data, 1, file_size, in_file);
    json_data[file_size] = '\0';
    cJSON *root = cJSON_Parse(json_data);
    delete[] json_data;
    assert_file_closed(&in_file);;

    if (root != NULL) {
        // Create a cJSON object and add it to root
        cJSON *object =  id_000034_createAndAddObject(root, "new_object");

        // Create a boolean value "true" and add it to object
        cJSON *true_value =  id_000034_createAndAddTrueValue(object, "new_boolean");

        // Create a string reference and set the value of object
        const char *valuestring = "new_string";
        char *string_reference =  id_000034_createStringReferenceAndSetValue(object, valuestring);

        // Compare root with itself
        cJSON_bool result =  id_000034_compareCJSONObjects(root, root, cJSON_False);

        // Print the cJSON object
        char *output_json = cJSON_Print(root);
        printf("Output JSON: %s\n", output_json);
        cJSON_free(output_json);

        // Delete the cJSON objects
        cJSON_Delete(root);

        // Free the string reference
        cJSON_free(string_reference);
    }

    assert_file_closed(&in_file);
	return 0;
}