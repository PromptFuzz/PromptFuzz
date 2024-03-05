#include "FDSan.h"
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


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *data, size_t size) {
	if(size<0) return 0;

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

  cJSON *widget = cJSON_GetObjectItem(root, "widget");
  if (widget == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *image = cJSON_GetObjectItem(widget, "image");
  if (image == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *text = cJSON_GetObjectItem(widget, "text");
  if (text == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  cJSON *value = cJSON_GetObjectItemCaseSensitive(text, "data");
  if (value == NULL) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  // Compare two cJSON objects
  cJSON *clone = cJSON_Duplicate(root, 1);
  cJSON_bool result = cJSON_Compare(root, clone, 1);
  cJSON_Delete(clone);
  if (result == 0) {
    cJSON_Delete(root);
    assert_file_closed(&in_file);
	return 0;
  }

  // Replace value in an object
  cJSON_ReplaceItemInObjectCaseSensitive(text, "data", cJSON_CreateString("New Data"));

  // Get the numeric value of an item
  double width = cJSON_GetNumberValue(cJSON_GetObjectItem(widget, "window.width"));

  // Add a boolean value to an object
  cJSON_AddBoolToObject(widget, "fullscreen", 1);

  // Insert an item into an array
  cJSON_InsertItemInArray(cJSON_GetObjectItem(widget, "text"), 0, cJSON_CreateString("First Item"));

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