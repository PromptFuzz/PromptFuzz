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
//<ID> 1606
//<Prompt> ["cre2_opt_new","cre2_set_new","cre2_opt_set_one_line","cre2_opt_set_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_new","cre2_program_size","cre2_consume_re","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"]
/*<Combination>: [cre2_options_t *cre2_opt_new(),
    cre2_set *cre2_set_new(cre2_options_t * opt, cre2_anchor_t anchor),
    void cre2_opt_set_one_line(cre2_options_t * opt, int flag),
    void cre2_opt_set_dot_nl(cre2_options_t * opt, int flag),
    cre2_regexp_t *cre2_new(const char * pattern, int pattern_len, const cre2_options_t * opt),
    cre2_named_groups_iter_t *cre2_named_groups_iter_new(const cre2_regexp_t * re),
    cre2_regexp_t *cre2_new(const char * pattern, int pattern_len, const cre2_options_t * opt),
    int cre2_program_size(const cre2_regexp_t * re),
    int cre2_consume_re(cre2_regexp_t * , cre2_string_t * , cre2_string_t * , int ),
    void cre2_set_delete(cre2_set * set),
    void cre2_delete(cre2_regexp_t * re),
    void cre2_named_groups_iter_delete(cre2_named_groups_iter_t * iter)
*/
//<score> 50, nr_unique_branch: 32
//<Quality> {"density":10,"unique_branches":{"_ZN3re28BitState4PushEiPKc":[[119,7,119,14,0,0,4,1],[119,18,119,27,0,0,4,0],[121,9,121,22,0,0,4,1]],"_ZN3re28BitState9TrySearchEiPKc":[[152,9,152,15,0,0,4,0],[173,7,173,21,0,0,4,0],[192,7,192,26,0,0,4,1],[206,7,206,24,0,0,4,0],[207,13,207,24,0,0,4,0],[207,13,207,24,0,0,4,1],[210,13,210,27,0,0,4,0],[210,31,210,54,0,0,4,0],[210,31,210,54,0,0,4,1],[219,7,219,27,0,0,4,0],[220,13,220,57,0,0,4,0],[220,13,220,57,0,0,4,1],[223,13,223,24,0,0,4,0],[223,13,223,24,0,0,4,1],[228,7,228,20,0,0,4,0],[229,13,229,24,0,0,4,0],[229,13,229,24,0,0,4,1],[237,13,237,31,0,0,4,0],[237,13,237,31,0,0,4,1],[241,7,241,22,0,0,4,0],[242,13,242,22,0,0,4,1],[247,13,247,28,0,0,4,1],[255,13,255,40,0,0,4,0],[257,27,257,41,0,0,4,0],[257,27,257,41,0,0,4,1],[264,13,264,22,0,0,4,0],[275,13,275,24,0,0,4,0]],"_ZN3re28BitState11ShouldVisitEiPKc":[[92,7,92,73,0,0,4,0]],"cre2_consume_re":[[443,21,443,29,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_opt_set_one_line","cre2_opt_set_dot_nl","cre2_set_new","cre2_new","cre2_program_size","cre2_consume_re","cre2_set_delete","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_one_line","cre2_opt_set_dot_nl","cre2_set_new","cre2_new","cre2_program_size","cre2_consume_re","cre2_set_delete","cre2_delete","cre2_opt_delete"],"visited":0}
/*Below is the code for the fuzz driver using cre2 library APIs:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Create and initialize options
    cre2_options_t *opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, fuzz_int32_t_1);
    cre2_opt_set_dot_nl(opt, fuzz_int32_t_2);
    
    // Create and initialize set
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    
    // Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create regular expression from input pattern
    char pattern[256];
    fgets(pattern, sizeof(pattern), in_file);
    cre2_regexp_t *regexp = cre2_new(pattern, sizeof(pattern), opt);
    
    // Get the program size of the regular expression
    int program_size = cre2_program_size(regexp);
    
    // Consume the input file using the regular expression
    cre2_string_t match;
    cre2_string_t replace;
    int nmatch = 1;
    cre2_string_t input_text;
    cre2_consume_re(regexp, &input_text, &replace, nmatch);
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Write program size and match to the output file
    fwrite(&program_size, sizeof(program_size), 1, out_file);
    fwrite(match.data, match.length, 1, out_file);
    
    // Clean up
    cre2_set_delete(set);
    cre2_delete(regexp);
    cre2_opt_delete(opt);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}