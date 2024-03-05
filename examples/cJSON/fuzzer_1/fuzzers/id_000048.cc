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
//<ID> 1580
//<Prompt> ["cJSON_Compare","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_GetNumberValue","cJSON_AddBoolToObject","cJSON_InsertItemInArray"]
/*<Combination>: [cJSON_bool cJSON_Compare(const cJSON *const a, const cJSON *const b, const cJSON_bool case_sensitive),
    cJSON_bool cJSON_ReplaceItemInObjectCaseSensitive(cJSON * object, const char * string, cJSON * newitem),
    double cJSON_GetNumberValue(const cJSON *const item),
    cJSON *cJSON_AddBoolToObject(cJSON *const object, const char *const name, const cJSON_bool boolean),
    cJSON_bool cJSON_InsertItemInArray(cJSON * array, int which, cJSON * newitem)
*/
//<score> 72, nr_unique_branch: 12
//<Quality> {"density":24,"unique_branches":{"cJSON_Compare":[[3019,9,3019,25,0,0,4,0],[3021,9,3021,24,0,0,4,0],[3044,9,3044,25,0,0,4,0],[3049,20,3049,39,0,0,4,0],[3049,20,3049,39,0,0,4,1],[3049,43,3049,62,0,0,4,0],[3049,43,3049,62,0,0,4,1],[3051,21,3051,73,0,0,4,0],[3051,21,3051,73,0,0,4,1],[3061,17,3061,39,0,0,4,0],[3061,17,3061,39,0,0,4,1]],"cJSON_IsNumber":[[2938,9,2938,21,0,0,4,0]]},"library_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItemCaseSensitive","cJSON_Delete","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_Delete","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateString","cJSON_GetNumberValue","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_InsertItemInArray","cJSON_GetObjectItem","cJSON_CreateString","cJSON_Delete","cJSON_Print","cJSON_Delete"],"critical_calls":["cJSON_Parse","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_GetObjectItemCaseSensitive","cJSON_Duplicate","cJSON_Compare","cJSON_Delete","cJSON_ReplaceItemInObjectCaseSensitive","cJSON_CreateString","cJSON_GetNumberValue","cJSON_GetObjectItem","cJSON_AddBoolToObject","cJSON_InsertItemInArray","cJSON_CreateString","cJSON_GetObjectItem","cJSON_Print","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<113) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
	//fuzzer shim end}




  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Read the input data from the file
  fseek(in_file, 0, SEEK_END);
  long file_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);
  char *json_data = (char *)malloc(file_size + 1);
  if (json_data == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
  }
  fread(json_data, 1, file_size, in_file);
  assert_file_closed(&in_file);;
  json_data[file_size] = '\0';

  cJSON *root = cJSON_Parse(json_data);
  free(json_data);

  if (root == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *widget = cJSON_GetObjectItem(root, fuzz_str_1);
  if (widget == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *image = cJSON_GetObjectItem(widget, fuzz_str_2);
  if (image == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *text = cJSON_GetObjectItem(widget, fuzz_str_3);
  if (text == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *value = cJSON_GetObjectItemCaseSensitive(text, fuzz_str_4);
  if (value == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  // Compare two cJSON objects
  cJSON *clone = cJSON_Duplicate(root, fuzz_int32_t_5);
  cJSON_bool result = cJSON_Compare(root, clone, fuzz_int32_t_6);
  cJSON_Delete(clone);
  if (result == 0) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  // Replace value in an object
  cJSON_ReplaceItemInObjectCaseSensitive(text, fuzz_str_7, cJSON_CreateString(fuzz_str_8));

  // Get the numeric value of an item
  double width = cJSON_GetNumberValue(cJSON_GetObjectItem(widget, fuzz_str_9));

  // Add a boolean value to an object
  cJSON_AddBoolToObject(widget, fuzz_str_10, fuzz_int32_t_11);

  // Insert an item into an array
  cJSON_InsertItemInArray(cJSON_GetObjectItem(widget, fuzz_str_13), fuzz_int32_t_12, cJSON_CreateString(fuzz_str_14));

  // Write the updated JSON data to a file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }
  char *output_json = cJSON_Print(root);
  if (output_json != NULL) {
    fwrite(output_json, 1, strlen(output_json), out_file);
    free(output_json);
  }
  assert_file_closed(&out_file);;

  cJSON_Delete(root);

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}