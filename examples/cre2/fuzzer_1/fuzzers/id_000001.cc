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
//<ID> 109
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
//<score> 36.666668, nr_unique_branch: 32
//<Quality> {"density":11,"unique_branches":{"_ZN3re23DFA17InlinedSearchLoopILb1ELb1ELb1EEEbPNS0_12SearchParamsE":[[1365,7,1365,19,0,0,4,1],[1384,10,1384,17,0,0,4,1],[1509,9,1509,56,0,0,4,0],[1521,7,1521,17,0,0,4,0],[1523,9,1523,19,0,0,4,1],[1538,7,1538,28,0,0,4,0],[1539,9,1539,24,0,0,4,0]],"_ZN3re23DFA14RunWorkqOnByteEPNS0_5WorkqES2_ijPb":[[954,9,954,26,0,0,4,0],[955,11,955,19,0,0,4,1]],"_ZN3re23DFA10AddToQueueEPNS0_5WorkqEij":[[854,9,854,19,0,0,4,0],[894,41,894,57,0,0,4,0],[895,13,895,44,0,0,4,0],[895,48,895,68,0,0,4,0]],"_ZN3re23DFA5Workq4markEv":[[386,9,386,23,0,0,4,0],[386,9,386,23,0,0,4,1]],"_ZN3re23DFAC2EPNS_4ProgENS1_9MatchKindEl":[[427,7,427,35,0,0,4,0]],"cre2_consume":[[372,19,372,27,0,0,4,1],[377,9,377,15,0,0,4,1]],"_ZN3re23DFA18WorkqToCachedStateEPNS0_5WorkqES2_j":[[626,9,626,23,0,0,4,0],[627,11,627,16,0,0,4,0],[627,20,627,37,0,0,4,0],[663,16,663,33,0,0,4,0],[699,7,699,35,0,0,4,0],[702,12,702,19,0,0,4,0],[702,12,702,19,0,0,4,1],[704,14,704,24,0,0,4,0],[704,14,704,24,0,0,4,1],[704,28,704,42,0,0,4,0],[707,11,707,21,0,0,4,1]],"_ZN3re24Prog6GetDFAENS0_9MatchKindE":[[1806,7,1806,26,0,0,4,1],[1811,14,1811,32,0,0,4,1]],"_ZZN3re24Prog6GetDFAENS0_9MatchKindEENK3$_2clEPS0_":[[1818,11,1818,27,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_case_sensitive","cre2_new","cre2_opt_delete","cre2_pattern","cre2_opt_encoding","cre2_consume","cre2_partial_match_re","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_case_sensitive","cre2_new","cre2_pattern","cre2_opt_encoding","cre2_consume","cre2_partial_match_re","cre2_delete","cre2_opt_delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




  // Create a file to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == nullptr) {
    assert_file_closed(&in_file);
	return 0;
  }
  
  // Create a file to write output data
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }
  
  // Get the file descriptors for reading and writing
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  
  // Create the regular expression options
  cre2_options_t *opt = cre2_opt_new();
  if (opt == nullptr) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // Set the options
  cre2_opt_set_posix_syntax(opt, fuzz_int32_t_1);
  cre2_opt_set_case_sensitive(opt, fuzz_int32_t_2);
  
  // Create the regular expression
  cre2_regexp_t *re = cre2_new(fuzz_str_3, static_cast<int>(fuzz_str_sz_3), opt);
  if (re == nullptr) {
    cre2_opt_delete(opt);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }
  
  // Get the regular expression pattern
  const char *pattern = cre2_pattern(re);
  printf("Pattern: %s\n", pattern);
  
  // Get the regular expression encoding
  cre2_encoding_t encoding = cre2_opt_encoding(opt);
  printf("Encoding: %d\n", encoding);
  
  // Define input and output strings
  cre2_string_t input;
  cre2_string_t output;
  input.data = nullptr;
  input.length = 0;
  output.data = nullptr;
  output.length = 0;
  
  // Consume the input data
  int result = cre2_consume(fuzz_str_4, &input, &output, 0);
  printf("Consume Result: %d\n", result);
  
  // Partial match with regular expression
  result = cre2_partial_match_re(re, &input, &output, fuzz_int32_t_5);
  printf("Partial Match Result: %d\n", result);
  
  // Release allocated resources
  cre2_delete(re);
  cre2_opt_delete(opt);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;
  
  assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}