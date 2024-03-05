#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 387
//<Prompt> ["cJSON_CreateStringReference","cJSON_ParseWithLength","cJSON_AddNumberToObject","cJSON_AddArrayToObject","cJSON_CreateObjectReference","cJSON_CreateObject","cJSON_AddObjectToObject","cJSON_AddItemReferenceToObject","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete"]
/*<Combination>: [cJSON *cJSON_CreateStringReference(const char * string),
    cJSON *cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON *cJSON_AddNumberToObject(cJSON *const object, const char *const name, const double number),
    cJSON *cJSON_AddArrayToObject(cJSON *const object, const char *const name),
    cJSON *cJSON_CreateObjectReference(const cJSON * child),
    cJSON *cJSON_CreateObject(),
    cJSON *cJSON_AddObjectToObject(cJSON *const object, const char *const name),
    cJSON_bool cJSON_AddItemReferenceToObject(cJSON * object, const char * string, cJSON * item),
    char *cJSON_PrintUnformatted(const cJSON * item),
    void cJSON_Minify(char * json),
    void cJSON_Delete(cJSON * item)
*/
//<score> 84, nr_unique_branch: 9
//<Quality> {"density":12,"unique_branches":{"print":[[1206,9,1206,35,0,0,4,0],[1238,9,1238,31,0,0,4,0],[1243,9,1243,24,0,0,4,1]],"cJSON_Minify":[[2842,9,2842,21,0,0,4,0]],"cJSON_AddItemReferenceToArray":[[2061,9,2061,22,0,0,4,0]],"skip_oneline_comment":[[2793,12,2793,31,0,0,4,1]],"cJSON_AddItemReferenceToObject":[[2071,9,2071,25,0,0,4,0]],"cJSON_AddArrayToObject":[[2178,9,2178,70,0,0,4,0],[2178,9,2178,70,0,0,4,1]]},"library_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddItemReferenceToObject","cJSON_AddArrayToObject","cJSON_CreateObjectReference","cJSON_AddItemReferenceToArray","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLength","cJSON_CreateObject","cJSON_AddNumberToObject","cJSON_AddItemReferenceToObject","cJSON_AddArrayToObject","cJSON_CreateObjectReference","cJSON_AddItemReferenceToArray","cJSON_PrintUnformatted","cJSON_Minify","cJSON_Delete","cJSON_Delete","cJSON_Delete"],"visited":0}
/*Here is an example of a C++ program that uses cJSON library APIs to achieve the desired event:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a file to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Read the input data from the file
    char *input_data = (char *)malloc(size + 1);
    fread(input_data, 1, size, in_file);
    input_data[size] = '\0';

    // Close the input file
    assert_file_closed(&in_file);;

    // Parse the input data using cJSON_ParseWithLength
    cJSON *root = cJSON_ParseWithLength(input_data, size);

    // Create a new cJSON object and add a number to it using cJSON_AddNumberToObject
    cJSON *number_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(number_obj, "number", 42);

    // Add the number_obj to the root object using cJSON_AddItemReferenceToObject
    cJSON_AddItemReferenceToObject(root, "number_obj", number_obj);

    // Create a new cJSON array using cJSON_AddArrayToObject
    cJSON *array_obj = cJSON_AddArrayToObject(root, "array_obj");

    // Create a cJSON object reference and add it to the array using cJSON_CreateObjectReference
    cJSON *object_ref = cJSON_CreateObjectReference(number_obj);
    cJSON_AddItemReferenceToArray(array_obj, object_ref);

    // Print the modified cJSON object using cJSON_PrintUnformatted
    char *modified_json = cJSON_PrintUnformatted(root);
    printf("Modified JSON: %s\n", modified_json);

    // Minify the modified JSON using cJSON_Minify
    cJSON_Minify(modified_json);
    printf("Minified JSON: %s\n", modified_json);

    // Delete the cJSON objects
    cJSON_Delete(root);
    cJSON_Delete(number_obj);
    cJSON_Delete(object_ref);

    // Free the memory
    free(input_data);
    free(modified_json);

    assert_file_closed(&in_file);
	return 0;
}