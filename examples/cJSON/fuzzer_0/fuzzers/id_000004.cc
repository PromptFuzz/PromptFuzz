#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 64
//<Prompt> ["cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_DetachItemFromArray","cJSON_PrintPreallocated"]
/*<Combination>: [cJSON *cJSON_DetachItemFromObjectCaseSensitive(cJSON * object, const char * string),
    cJSON_bool cJSON_IsNull(const cJSON *const item),
    cJSON_bool cJSON_IsArray(const cJSON *const item),
    cJSON *cJSON_AddStringToObject(cJSON *const object, const char *const name, const char *const string),
    cJSON *cJSON_DetachItemFromObject(cJSON * object, const char * string),
    cJSON *cJSON_DetachItemFromArray(cJSON * array, int which),
    cJSON_bool cJSON_PrintPreallocated(cJSON * item, char * buffer, const int length, const cJSON_bool format)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["cJSON_Parse","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_Delete","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_GetArrayItem","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_DetachItemFromObjectCaseSensitive","cJSON_IsNull","cJSON_Delete","cJSON_IsArray","cJSON_AddStringToObject","cJSON_DetachItemFromObject","cJSON_Delete","cJSON_GetArrayItem","cJSON_IsArray","cJSON_DetachItemFromArray","cJSON_Delete","cJSON_PrintPreallocated","cJSON_Delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* data, size_t size) {
	if(size<0) return 0;


  // Create a file stream to read input data
  FILE* in_file = fmemopen((void*)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Read the input data from file
  fseek(in_file, 0, SEEK_END);
  long file_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  char* file_data = (char*)malloc(file_size + 1);
  size_t bytes_read = fread(file_data, 1, file_size, in_file);
  assert_file_closed(&in_file);;
  if (bytes_read != file_size) {
    free(file_data);
    assert_file_closed(&in_file);
	return 0;
  }
  file_data[file_size] = '\0';

  // Parse the input data as a cJSON object
  cJSON* root = cJSON_Parse(file_data);
  free(file_data);
  if (!root) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Detach an item from the object
  cJSON* item = cJSON_DetachItemFromObjectCaseSensitive(root, "value");
  if (item) {
    // Check if the detached item is null
    cJSON_bool is_null = cJSON_IsNull(item);
    cJSON_Delete(item);
  }

  // Check if the root object is an array
  cJSON_bool is_array = cJSON_IsArray(root);

  // Add a string item to the root object
  cJSON* string_item = cJSON_AddStringToObject(root, "new_value", "Hello World");

  // Detach an item from the root object
  cJSON* detached_item = cJSON_DetachItemFromObject(root, "menu");
  if (detached_item) {
    cJSON_Delete(detached_item);
  }

  // Detach an item from an array
  cJSON* array = cJSON_GetArrayItem(root, 0);
  if (cJSON_IsArray(array)) {
    cJSON* array_item = cJSON_DetachItemFromArray(array, 0);
    if (array_item) {
      cJSON_Delete(array_item);
    }
  }

  // Print the cJSON object to a pre-allocated buffer
  int buffer_size = 1024;
  char* buffer = (char*)malloc(buffer_size);
  cJSON_PrintPreallocated(root, buffer, buffer_size, true);
  free(buffer);

  // Delete the cJSON object
  cJSON_Delete(root);

  assert_file_closed(&in_file);
	return 0;
}