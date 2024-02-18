#include "FuzzedDataProvider.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
//<Parent> None
/*<Combination>: [cJSON_bool cJSON_AddItemReferenceToArray(cJSON * array, cJSON * item),
    char * cJSON_GetStringValue(const cJSON * item),
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item),
    cJSON_bool cJSON_IsNumber(const cJSON * item),
    cJSON * cJSON_AddArrayToObject(cJSON * object, const char * name),
    cJSON * cJSON_ParseWithLength(const char * value, size_t buffer_length),
    cJSON * cJSON_AddRawToObject(cJSON * object, const char * name, const char * raw),
    cJSON_bool cJSON_AddItemToArray(cJSON * array, cJSON * item),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    cJSON * cJSON_DetachItemFromArray(cJSON * array, int which),
    double cJSON_GetNumberValue(const cJSON * item)
*/
//<Prompt> Prompt { kind: Generate } 
//<Mutator> Mutator { op: NoMutation, response: None }
#include <iostream>
#include <fstream>
#include <vector>
#include "cJSON.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* f_data, size_t f_size) {
	if(f_size<62) return 0;

	
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, data_sz, fdp)
		FDPConsumeRandomLengthString(fuzz_str_1, fdp);
		FDPConsumeRandomLengthString(fuzz_str_2, fdp);
		FDPConsumeRandomLengthString(fuzz_str_3, fdp);
		FDPConsumeRandomLengthString(fuzz_str_4, fdp);
		FDPConsumeRandomLengthString(fuzz_str_5, fdp);
		FDPConsumeRandomLengthString(fuzz_str_6, fdp);
		FDPConsumeRandomLengthString(fuzz_str_7, fdp);
	
		const char *fuzzer_var_cJSON_AddObjectToObject_3_1 = fuzz_str_1;
		const char *fuzzer_var_cJSON_AddItemToObject_8_1 = fuzz_str_2;
		const char * str_val = fuzz_str_3;
		const char *fuzzer_var_cJSON_ReplaceItemInObject_12_1 = fuzz_str_4;
		const char *fuzzer_var_cJSON_GetObjectItem_15_1 = fuzz_str_5;
		const char *fuzzer_var_cJSON_AddRawToObject_20_1 = fuzz_str_6;
		const char * raw_val = fuzz_str_7;//fuzzer shim end}








    // Parsing JSON file
    cJSON *root = cJSON_ParseWithLength(reinterpret_cast<const char *>(data), data_sz);
    if (!root) {
        std::cerr << "Failed to parse input data as JSON" << std::endl;
        return 0;
    }

    // Create JSON Array
    cJSON *json_array = cJSON_CreateArray();
    if (!json_array) {
        std::cerr << "Failed to create JSON array" << std::endl;
        cJSON_Delete(root);
        return 0;
    }

    // Add JSON Object to Array
    cJSON *json_object = cJSON_AddObjectToObject(json_array, fuzzer_var_cJSON_AddObjectToObject_3_1);
    if (!json_object) {
        std::cerr << "Failed to create JSON object" << std::endl;
        cJSON_Delete(json_array);
        cJSON_Delete(root);
        return 0;
    }

    // Add Number to JSON Array
    cJSON_AddItemToArray(json_array, cJSON_CreateNumber(1));

    // Add String to JSON Object
    
    if (!cJSON_AddItemToObject(json_object, fuzzer_var_cJSON_AddItemToObject_8_1, cJSON_CreateString(str_val))) {
        std::cerr << "Failed to add string to JSON object" << std::endl;
        cJSON_Delete(json_array);
        cJSON_Delete(root);
        return 0;
    }

    // Replace JSON Object in root
    if (!cJSON_ReplaceItemInObject(root, fuzzer_var_cJSON_ReplaceItemInObject_12_1, json_array))
    {
        std::cerr << "Failed to replace JSON object in root" << std::endl;
        cJSON_Delete(json_array);
        cJSON_Delete(root);
        return 0;
    }

    // Get String Value from JSON Object
    cJSON *json_str = cJSON_GetObjectItem(json_object, fuzzer_var_cJSON_GetObjectItem_15_1);
    if (cJSON_IsString(json_str)) {
        char *str = cJSON_GetStringValue(json_str);
        std::cout << "Name: " << str << std::endl;
    }

    // Detach item from Array
    cJSON *item = cJSON_DetachItemFromArray(json_array, 1);
    if (item) {
        cJSON_Delete(item);
    }

    // Add Raw value to JSON Object
    
    if (!cJSON_AddRawToObject(json_object, fuzzer_var_cJSON_AddRawToObject_20_1, raw_val)) {
        std::cerr << "Failed to add raw value to JSON object" << std::endl;
        cJSON_Delete(json_array);
        cJSON_Delete(root);
        return 0;
    }

    // Get Number Value from JSON Array
    cJSON *json_num = cJSON_GetArrayItem(json_array, 0);
    if (cJSON_IsNumber(json_num)) {
        double num = cJSON_GetNumberValue(json_num);
        std::cout << "Number: " << num << std::endl;
    }

    // Add Reference to Array
    if (!cJSON_AddItemReferenceToArray(json_array, json_object)) {
        std::cerr << "Failed to add reference to JSON array" << std::endl;
        cJSON_Delete(json_array);
        cJSON_Delete(root);
        return 0;
    }

    // Release Allocated Resources
    cJSON_Delete(json_array);
    cJSON_Delete(root);

    return 0;
}