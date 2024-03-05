#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 105
//<Prompt> ["cre2_version_interface_current","cre2_version_interface_age","cre2_partial_match_re","cre2_opt_dot_nl","cre2_pattern","cre2_opt_set_posix_syntax","cre2_opt_encoding","cre2_consume","cre2_opt_set_case_sensitive"]
/*<Combination>: [int cre2_version_interface_current(),
    int cre2_version_interface_age(),
    int cre2_partial_match_re(cre2_regexp_t * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_opt_dot_nl(cre2_options_t * opt),
    const char *cre2_pattern(const cre2_regexp_t * re),
    void cre2_opt_set_posix_syntax(cre2_options_t * opt, int flag),
    cre2_encoding_t cre2_opt_encoding(cre2_options_t * opt),
    int cre2_consume(const char * , cre2_string_t * , cre2_string_t * , int ),
    void cre2_opt_set_case_sensitive(cre2_options_t * opt, int flag)
*/
//<score> 17.333334, nr_unique_branch: 7
//<Quality> {"density":13,"unique_branches":{"_ZNK3re23RE27Options10ParseFlagsEv":[[185,7,185,24,0,0,4,0]],"cre2_partial_match_re":[[406,19,406,27,0,0,4,0],[406,19,406,27,0,0,4,1],[411,9,411,15,0,0,4,1]],"cre2_opt_encoding":[[117,3,117,34,0,0,4,0],[119,3,119,36,0,0,4,1],[121,3,121,10,0,0,4,1]]},"library_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_case_sensitive","cre2_new","cre2_pattern","cre2_version_interface_current","cre2_version_interface_age","cre2_opt_encoding","cre2_delete","cre2_opt_delete","cre2_delete","cre2_opt_delete","cre2_partial_match_re","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_case_sensitive","cre2_new","cre2_pattern","cre2_version_interface_current","cre2_version_interface_age","cre2_opt_encoding","cre2_partial_match_re","cre2_delete","cre2_opt_delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Step 4: Open input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get file descriptor for input file
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Use input file name as needed
  const char *input_file = "input_file";

  // Step 7: Open output file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 8: Get file descriptor for output file
  int out_fd = fuzz_fileno(out_file);

  // Step 9: Use output file name as needed
  const char *output_file = "output_file";

  // Step 11: Initialize cre2 options
  cre2_options_t *opt = cre2_opt_new();

  // Step 12: Set options as needed
  cre2_opt_set_posix_syntax(opt, fuzz_int32_t_1);
  cre2_opt_set_case_sensitive(opt, fuzz_int32_t_2);

  // Step 13: Create regex pattern
  const char *pattern = "regex_pattern";
  cre2_regexp_t *re = cre2_new(fuzz_str_3, static_cast<int>(fuzz_str_sz_3), opt);

  // Step 14: Get regex pattern
  const char *regex_pattern = cre2_pattern(re);

  // Step 15: Get cre2 version interface current
  int version_interface_current = cre2_version_interface_current();

  // Step 16: Get cre2 version interface age
  int version_interface_age = cre2_version_interface_age();

  // Step 17: Get cre2 encoding
  cre2_encoding_t encoding = cre2_opt_encoding(opt);

  // Step 18: Initialize cre2 string
  cre2_string_t input_str;
  input_str.data = NULL;
  input_str.length = 0;

  // Step 19: Read input data from file
  fseek(in_file, 0, SEEK_END);
  long input_size = ftell(in_file);
  fseek(in_file, 0, SEEK_SET);

  input_str.data = (const char *)malloc(input_size);
  if (input_str.data == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cre2_delete(re);
    cre2_opt_delete(opt);
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  fread((void *)input_str.data, 1, input_size, in_file);
  input_str.length = (int)input_size;

  // Step 20: Initialize cre2 string for output
  cre2_string_t output_str;
  output_str.data = (const char *)malloc(input_size);
  if (output_str.data == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cre2_delete(re);
    cre2_opt_delete(opt);
    free((void *)input_str.data);
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  output_str.length = (int)input_size;

  // Step 21: Call cre2_partial_match_re
  int match_count = cre2_partial_match_re(re, &input_str, &output_str, 1);

  // Step 22: Write output data to file
  fwrite((const void *)output_str.data, 1, output_str.length, out_file);

  // Step 23: Release allocated resources
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  cre2_delete(re);
  cre2_opt_delete(opt);
  free((void *)input_str.data);
  free((void *)output_str.data);

  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}