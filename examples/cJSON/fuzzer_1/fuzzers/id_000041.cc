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
//<ID> 1188
//<Prompt> ["cJSON_CreateFloatArray","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_CreateIntArray","cJSON_AddFalseToObject","cJSON_Print","cJSON_free","cJSON_IsBool","cJSON_ParseWithOpts","cJSON_Delete"]
/*<Combination>: [cJSON *cJSON_CreateFloatArray(const float * numbers, int count),
    cJSON *cJSON_CreateDoubleArray(const double * numbers, int count),
    cJSON_bool cJSON_AddItemToObject(cJSON * object, const char * string, cJSON * item),
    cJSON *cJSON_AddRawToObject(cJSON *const object, const char *const name, const char *const raw),
    cJSON *cJSON_CreateIntArray(const int * numbers, int count),
    cJSON *cJSON_AddFalseToObject(cJSON *const object, const char *const name),
    char *cJSON_Print(const cJSON * item),
    void cJSON_free(void * object),
    cJSON_bool cJSON_IsBool(const cJSON *const item),
    cJSON *cJSON_ParseWithOpts(const char * value, const char ** return_parse_end, cJSON_bool require_null_terminated),
    void cJSON_Delete(cJSON * item)
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"cJSON_CreateFloatArray":[[2592,24,2592,41,0,0,4,0]]},"library_calls":["cJSON_ParseWithOpts","cJSON_GetObjectItem","cJSON_Delete","cJSON_GetObjectItem","cJSON_Delete","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_Delete","cJSON_AddFalseToObject","cJSON_Delete","cJSON_Print","cJSON_Delete","cJSON_Delete"],"critical_calls":["cJSON_ParseWithOpts","cJSON_GetObjectItem","cJSON_GetObjectItem","cJSON_CreateFloatArray","cJSON_AddItemToObject","cJSON_CreateDoubleArray","cJSON_AddItemToObject","cJSON_CreateIntArray","cJSON_AddItemToObject","cJSON_AddRawToObject","cJSON_AddFalseToObject","cJSON_Print","cJSON_Delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<94) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




  FILE *in_file;
  cJSON *json_object;
  in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    return 0;
  }

  fseek(in_file, 0, SEEK_END);
  long file_size = ftell(in_file);
  rewind(in_file);

  char *json_data = (char *)malloc(file_size + 1);
  if (!json_data) {
    assert_file_closed(&in_file);;
    return 0;
  }

  size_t bytes_read = fread(json_data, 1, file_size, in_file);
  if (bytes_read != (size_t)file_size) {
    free(json_data);
    assert_file_closed(&in_file);;
    return 0;
  }

  assert_file_closed(&in_file);;
  json_data[file_size] = '\0';

  const char *parse_end;
  json_object = cJSON_ParseWithOpts(json_data, &parse_end, fuzz_int32_t_1);
  
  free(json_data);

  if (!json_object) {
    return 0;
  }

  cJSON *object_item = cJSON_GetObjectItem(json_object, fuzz_str_2);
  if (!object_item) {
    cJSON_Delete(json_object);
    return 0;
  }

  cJSON *array_item = cJSON_GetObjectItem(object_item, fuzz_str_3);
  if (!array_item) {
    cJSON_Delete(json_object);
    return 0;
  }

  cJSON *float_array = cJSON_CreateFloatArray(NULL, 0);
  cJSON_AddItemToObject(object_item, fuzz_str_4, float_array);

  cJSON *double_array = cJSON_CreateDoubleArray(NULL, 0);
  cJSON_AddItemToObject(object_item, fuzz_str_5, double_array);

  cJSON *int_array = cJSON_CreateIntArray(NULL, 0);
  cJSON_AddItemToObject(array_item, fuzz_str_6, int_array);

  cJSON *raw_item = cJSON_AddRawToObject(array_item, fuzz_str_7, fuzz_str_8);
  if (!raw_item) {
    cJSON_Delete(json_object);
    return 0;
  }

  cJSON *false_item = cJSON_AddFalseToObject(array_item, fuzz_str_9);
  if (!false_item) {
    cJSON_Delete(json_object);
    return 0;
  }

  char *json_string = cJSON_Print(json_object);
  if (!json_string) {
    cJSON_Delete(json_object);
    return 0;
  }
  free(json_string);

  cJSON_Delete(json_object);

  return 0;
}