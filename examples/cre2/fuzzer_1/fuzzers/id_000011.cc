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
//<ID> 1236
//<Prompt> ["cre2_set_add","cre2_find_and_consume","cre2_full_match","cre2_partial_match","cre2_consume","cre2_strings_to_ranges"]
/*<Combination>: [int cre2_set_add(cre2_set * set, const char * pattern, size_t pattern_len, char * error, size_t error_len),
    int cre2_find_and_consume(const char * , cre2_string_t * , cre2_string_t * , int ),
    int cre2_full_match(const char * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_partial_match(const char * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_consume(const char * , cre2_string_t * , cre2_string_t * , int ),
    void cre2_strings_to_ranges(const char * text, cre2_range_t * ranges, cre2_string_t * strings, int nmatch)
*/
//<score> 260, nr_unique_branch: 108
//<Quality> {"density":13,"unique_branches":{"_ZN3re24Prog4Inst6greedyEPS0_":[[96,14,96,56,0,0,4,1],[97,15,97,51,0,0,4,0],[98,15,98,73,0,0,4,0]],"_ZN3re23DFA17InlinedSearchLoopILb0ELb1ELb0EEEbPNS0_12SearchParamsE":[[1365,7,1365,19,0,0,4,1],[1384,10,1384,17,0,0,4,0],[1424,9,1424,19,0,0,4,0],[1426,11,1426,21,0,0,4,1],[1466,9,1466,30,0,0,4,0],[1466,9,1466,30,0,0,4,1],[1467,11,1467,26,0,0,4,0],[1477,9,1477,21,0,0,4,0],[1477,9,1477,21,0,0,4,1],[1487,11,1487,34,0,0,4,1]],"_ZN3re23DFA21RunWorkqOnEmptyStringEPNS0_5WorkqES2_j":[[937,9,937,26,0,0,4,0]],"_ZN3re24Prog20PrefixAccel_ShiftDFAEPKvm":[[1033,7,1033,26,0,0,4,0],[1033,7,1033,26,0,0,4,1],[1040,7,1040,16,0,0,4,0],[1040,7,1040,16,0,0,4,1],[1071,11,1071,45,0,0,4,0],[1071,11,1071,45,0,0,4,1],[1076,13,1076,38,0,0,4,0],[1076,13,1076,38,0,0,4,1],[1077,13,1077,38,0,0,4,0],[1077,13,1077,38,0,0,4,1],[1078,13,1078,38,0,0,4,0],[1078,13,1078,38,0,0,4,1],[1079,13,1079,38,0,0,4,0],[1079,13,1079,38,0,0,4,1],[1080,13,1080,38,0,0,4,0],[1080,13,1080,38,0,0,4,1],[1081,13,1081,38,0,0,4,0],[1081,13,1081,38,0,0,4,1],[1082,13,1082,38,0,0,4,0],[1082,13,1082,38,0,0,4,1],[1083,13,1083,38,0,0,4,0],[1088,14,1088,23,0,0,4,0],[1088,14,1088,23,0,0,4,1],[1095,10,1095,19,0,0,4,0],[1095,10,1095,19,0,0,4,1],[1099,9,1099,42,0,0,4,0],[1099,9,1099,42,0,0,4,1]],"cre2_find_and_consume":[[377,9,377,15,0,0,4,0],[380,21,380,29,0,0,4,1]],"cre2_partial_match":[[348,9,348,15,0,0,4,0],[349,21,349,29,0,0,4,1]],"_ZN3re23DFA17InlinedSearchLoopILb1ELb0ELb1EEEbPNS0_12SearchParamsE":[[1466,9,1466,30,0,0,4,0],[1467,11,1467,26,0,0,4,0],[1467,11,1467,26,0,0,4,1],[1477,9,1477,21,0,0,4,0],[1487,11,1487,34,0,0,4,1],[1538,7,1538,28,0,0,4,1],[1549,7,1549,19,0,0,4,0],[1554,9,1554,32,0,0,4,1]],"_ZN3re23DFA18WorkqToCachedStateEPNS0_5WorkqES2_j":[[624,52,624,66,0,0,4,0],[635,7,635,25,0,0,4,0],[640,13,640,38,0,0,4,0],[641,14,641,40,0,0,4,0],[641,14,641,40,0,0,4,1],[642,15,642,31,0,0,4,0],[642,15,642,31,0,0,4,1],[642,35,642,52,0,0,4,0],[643,14,643,42,0,0,4,0],[643,14,643,42,0,0,4,1],[643,46,643,54,0,0,4,0],[644,13,644,32,0,0,4,0],[644,13,644,32,0,0,4,1],[650,7,650,14,0,0,4,1],[657,43,657,63,0,0,4,1]],"_ZN3re23DFA10AddToQueueEPNS0_5WorkqEij":[[900,7,900,25,0,0,4,0]],"_ZN3re23NFA11AllocThreadEv":[[160,7,160,16,0,0,4,0]],"_ZN3re26Regexp13ComputeSimpleEv":[[73,31,73,43,0,0,4,1]],"_ZN3re2L8TopEqualEPNS_6RegexpES1_":[[419,14,420,79,0,0,4,1]],"_ZN3re24Prog11PrefixAccelEPKvm":[[225,9,225,25,0,0,4,0],[227,16,227,33,0,0,4,1]],"_ZN3re23DFA14RunWorkqOnByteEPNS0_5WorkqES2_ijPb":[[955,11,955,19,0,0,4,0],[970,7,970,25,0,0,4,0],[978,13,978,28,0,0,4,0],[993,36,993,53,0,0,4,0],[994,13,994,38,0,0,4,0]],"_ZN3re23DFA17InlinedSearchLoopILb1ELb1ELb1EEEbPNS0_12SearchParamsE":[[1466,9,1466,30,0,0,4,0],[1467,11,1467,26,0,0,4,1],[1477,9,1477,21,0,0,4,0],[1487,11,1487,34,0,0,4,1],[1538,7,1538,28,0,0,4,1],[1549,7,1549,19,0,0,4,0],[1554,9,1554,32,0,0,4,1]],"_ZN3re23DFA17InlinedSearchLoopILb0ELb1ELb1EEEbPNS0_12SearchParamsE":[[1384,10,1384,17,0,0,4,1],[1424,9,1424,19,0,0,4,1],[1466,9,1466,30,0,0,4,1],[1467,11,1467,26,0,0,4,1],[1477,9,1477,21,0,0,4,0],[1477,9,1477,21,0,0,4,1],[1487,11,1487,34,0,0,4,1],[1509,9,1509,56,0,0,4,0],[1521,7,1521,17,0,0,4,0],[1523,9,1523,19,0,0,4,1],[1538,7,1538,28,0,0,4,0],[1538,7,1538,28,0,0,4,1],[1539,9,1539,24,0,0,4,0],[1549,7,1549,19,0,0,4,0],[1554,9,1554,32,0,0,4,1]],"_ZN3re2L16ascii_strcasecmpEPKcS1_m":[[634,10,634,16,0,0,4,0],[637,9,637,17,0,0,4,0],[637,21,637,29,0,0,4,1],[639,9,639,15,0,0,4,0]],"cre2_full_match":[[348,9,348,15,0,0,4,0],[349,21,349,29,0,0,4,1]],"_ZN3re23DFA17InlinedSearchLoopILb0ELb0ELb1EEEbPNS0_12SearchParamsE":[[1467,11,1467,26,0,0,4,1]]},"library_calls":["cre2_opt_new","cre2_opt_set_log_errors","cre2_opt_set_dot_nl","cre2_opt_set_never_nl","cre2_set_new","cre2_set_add","cre2_full_match","cre2_partial_match","cre2_consume","cre2_find_and_consume","cre2_strings_to_ranges","cre2_set_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_log_errors","cre2_opt_set_dot_nl","cre2_opt_set_never_nl","cre2_set_new","cre2_set_add","cre2_full_match","cre2_partial_match","cre2_consume","cre2_find_and_consume","cre2_strings_to_ranges","cre2_set_delete","cre2_opt_delete"],"visited":0}
/*Here is the implementation of the fuzz driver using the cre2 library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Validate file handles
    if (in_file == nullptr || out_file == nullptr) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize cre2_options_t
    cre2_options_t *opt = cre2_opt_new();

    // Set options
    cre2_opt_set_log_errors(opt, fuzz_int32_t_1);
    cre2_opt_set_dot_nl(opt, fuzz_int32_t_2);
    cre2_opt_set_never_nl(opt, fuzz_int32_t_3);

    // Initialize cre2_set
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);

    // Read pattern from file
    char pattern[1024];
    fgets(pattern, sizeof(pattern), in_file);

    // Add pattern to set
    cre2_set_add(set, pattern, strlen(pattern), nullptr, 0);

    // Set up cre2_string_t variables for matching
    cre2_string_t text;
    text.data = pattern;
    text.length = strlen(pattern);

    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    // Full match
    cre2_full_match(pattern, &text, &match, 0);

    // Partial match
    cre2_partial_match(pattern, &text, &match, 0);

    // Consume
    cre2_consume(pattern, &text, &match, 0);

    // Find and consume
    cre2_find_and_consume(pattern, &text, &match, 0);

    // Convert strings to ranges
    cre2_range_t ranges[10];
    cre2_string_t strings[10];

    cre2_strings_to_ranges(pattern, ranges, strings, 10);

    // Write output to file
    fwrite(match.data, 1, match.length, out_file);

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}