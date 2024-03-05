#include "FDSan.h"
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 115
//<Prompt> ["cJSON_ParseWithLengthOpts","cJSON_PrintBuffered","cJSON_CreateRaw","cJSON_IsRaw","cJSON_ReplaceItemInObject","cJSON_DeleteItemFromObject"]
/*<Combination>: [cJSON *cJSON_ParseWithLengthOpts(const char * value, size_t buffer_length, const char ** return_parse_end, cJSON_bool require_null_terminated),
    char *cJSON_PrintBuffered(const cJSON * item, int prebuffer, cJSON_bool fmt),
    cJSON *cJSON_CreateRaw(const char * raw),
    cJSON_bool cJSON_IsRaw(const cJSON *const item),
    cJSON_bool cJSON_ReplaceItemInObject(cJSON * object, const char * string, cJSON * newitem),
    void cJSON_DeleteItemFromObject(cJSON * object, const char * string)
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["cJSON_ParseWithLengthOpts","cJSON_PrintBuffered","cJSON_Delete"],"critical_calls":["cJSON_ParseWithLengthOpts","cJSON_PrintBuffered","cJSON_Delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  cJSON *root = NULL;
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Read input data from file
  fseek(in_file, 0, SEEK_END);
  long file_size = ftell(in_file);
  rewind(in_file);
  char *file_content = (char*)calloc(file_size + 1, sizeof(char));
  fread(file_content, sizeof(char), file_size, in_file);
  assert_file_closed(&in_file);;

  // Parse the input data using cJSON_ParseWithLengthOpts
  const char *parse_end;
  root = cJSON_ParseWithLengthOpts(file_content, file_size, &parse_end, cJSON_True);
  free(file_content);
  if (root == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Print the parsed cJSON object using cJSON_PrintBuffered
  char *output = cJSON_PrintBuffered(root, 1024, cJSON_True);
  cJSON_Delete(root);
  
  // Write output data to file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file != NULL) {
    fwrite(output, sizeof(char), strlen(output), out_file);
    assert_file_closed(&out_file);;
  }
  free(output);
    
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}