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


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}





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
  cJSON* item = cJSON_DetachItemFromObjectCaseSensitive(root, fuzz_str_1);
  if (item) {
    // Check if the detached item is null
    cJSON_bool is_null = cJSON_IsNull(item);
    cJSON_Delete(item);
  }

  // Check if the root object is an array
  cJSON_bool is_array = cJSON_IsArray(root);

  // Add a string item to the root object
  cJSON* string_item = cJSON_AddStringToObject(root, fuzz_str_2, fuzz_str_3);

  // Detach an item from the root object
  cJSON* detached_item = cJSON_DetachItemFromObject(root, fuzz_str_4);
  if (detached_item) {
    cJSON_Delete(detached_item);
  }

  // Detach an item from an array
  cJSON* array = cJSON_GetArrayItem(root, fuzz_int32_t_5);
  if (cJSON_IsArray(array)) {
    cJSON* array_item = cJSON_DetachItemFromArray(array, fuzz_int32_t_6);
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