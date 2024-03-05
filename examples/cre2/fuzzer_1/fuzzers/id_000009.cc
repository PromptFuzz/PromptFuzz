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
//<ID> 1126
//<Prompt> ["cre2_find_and_consume","cre2_global_replace","cre2_set_compile","cre2_find_named_capturing_groups","cre2_full_match","cre2_set_match"]
/*<Combination>: [int cre2_find_and_consume(const char * , cre2_string_t * , cre2_string_t * , int ),
    int cre2_global_replace(const char * pattern, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_set_compile(cre2_set * set),
    int cre2_find_named_capturing_groups(const cre2_regexp_t * re, const char * name),
    int cre2_full_match(const char * , const cre2_string_t * , cre2_string_t * , int ),
    size_t cre2_set_match(cre2_set * set, const char * text, size_t text_len, int * match, size_t match_len)
*/
//<score> 35, nr_unique_branch: 12
//<Quality> {"density":10,"unique_branches":{"_ZN3re214SimplifyWalker9PostVisitEPNS_6RegexpES2_S2_PS2_i":[[482,5,482,26,0,0,4,0]],"_ZN3re28Compiler9PostVisitEPNS_6RegexpENS_4FragES3_PS3_i":[[856,5,856,26,0,0,4,0],[858,11,858,38,0,0,4,1]],"_ZN3re23DFA17InlinedSearchLoopILb0ELb1ELb1EEEbPNS0_12SearchParamsE":[[1365,7,1365,19,0,0,4,1],[1384,10,1384,17,0,0,4,0],[1424,9,1424,19,0,0,4,0],[1426,11,1426,21,0,0,4,1],[1466,9,1466,30,0,0,4,0],[1467,11,1467,26,0,0,4,0]],"_ZN3re23DFA18WorkqToCachedStateEPNS0_5WorkqES2_j":[[716,7,716,32,0,0,4,0]],"_ZN3re23RE23Set7CompileEv":[[108,19,108,28,0,0,4,0]],"_ZN3re23DFA19AnalyzeSearchHelperEPNS0_12SearchParamsEPNS0_9StartInfoEj":[[1727,7,1727,20,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_opt_set_never_nl","cre2_opt_set_log_errors","cre2_set_new","cre2_set_add","cre2_set_add","cre2_set_compile","cre2_set_match","cre2_set_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_never_nl","cre2_opt_set_log_errors","cre2_set_new","cre2_set_add","cre2_set_add","cre2_set_compile","cre2_set_match","cre2_set_delete","cre2_opt_delete"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<43) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 5: Get the file descriptor for reading
    int fd_in = fuzz_fileno(in_file);

    // Step 7: Release the resources before return
    assert_file_closed(&in_file);;

    // Step 6: Use the file name "output_file" for output file
    const char *output_file = "output_file";

    // Step 4: Create a FILE* for writing output data
    FILE *out_file = fopen(output_file, "wb");

    // Step 5: Get the file descriptor for writing
    int fd_out = fuzz_fileno(out_file);

    // Step 7: Release the resources before return
    assert_file_closed(&out_file);;

    // Step 1: Define the event that we want to achieve

    // 1. Initialize cre2 options
    cre2_options_t *opt = cre2_opt_new();

    // 2. Set the options as required
    cre2_opt_set_never_nl(opt, fuzz_int32_t_1);
    cre2_opt_set_log_errors(opt, fuzz_int32_t_2);

    // 3. Create a cre2 set
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);

    // 4. Add patterns to the set
    const char *pattern1 = "pattern1";
    cre2_set_add(set, fuzz_str_3, static_cast<size_t>(fuzz_str_sz_3), NULL, 0);

    const char *pattern2 = "pattern2";
    cre2_set_add(set, fuzz_str_4, static_cast<size_t>(fuzz_str_sz_4), NULL, 0);

    // 5. Compile the set
    cre2_set_compile(set);

    // 6. Match the set against input text
    const char *input_text = "input text";
    size_t input_text_len = strlen(input_text);

    int match[2];
    size_t num_matches = cre2_set_match(set, fuzz_str_5, static_cast<size_t>(fuzz_str_sz_5), match, 2);

    // Step 7: Release the resources before return
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}