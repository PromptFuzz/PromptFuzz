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
//<ID> 1869
//<Prompt> ["cre2_replace","cre2_find_named_capturing_groups","cre2_set_add","cre2_replace_re","cre2_partial_match","cre2_match"]
/*<Combination>: [int cre2_replace(const char * pattern, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_find_named_capturing_groups(const cre2_regexp_t * re, const char * name),
    int cre2_set_add(cre2_set * set, const char * pattern, size_t pattern_len, char * error, size_t error_len),
    int cre2_replace_re(cre2_regexp_t * rex, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_partial_match(const char * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_match(const cre2_regexp_t * re, const char * text, int textlen, int startpos, int endpos, cre2_anchor_t anchor, cre2_string_t * match, int nmatch)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["cre2_opt_new","cre2_opt_set_log_errors","cre2_new","cre2_error_code","cre2_error_string","cre2_delete","cre2_opt_delete","cre2_match","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_log_errors","cre2_new","cre2_error_code","cre2_match","cre2_delete","cre2_opt_delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Create a CRE2 regular expression pattern
    const char* pattern = "([a-z]+)";
    int pattern_len = strlen(pattern);
    
    // Create a CRE2 options object and set the necessary options
    cre2_options_t* opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, fuzz_int32_t_1);
    
    // Create a CRE2 regular expression object
    cre2_regexp_t* re = cre2_new(fuzz_str_2, static_cast<int>(fuzz_str_sz_2), opt);
    
    // Check if the regular expression compilation was successful
    if (cre2_error_code(re) != 0) {
        // Error occurred, handle the error
        std::cout << "Regular expression compilation failed: " << cre2_error_string(re) << std::endl;
        
        // Cleanup and return
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 0;
    }
    
    // Create a CRE2 string object with input data
    cre2_string_t input;
    input.data = reinterpret_cast<const char*>(data);
    input.length = size;
    
    // Create a CRE2 string object for the match
    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;
    
    // Find the first match in the input data using the regular expression
    int num_matches = cre2_match(re, input.data, input.length, fuzz_int32_t_3, input.length, CRE2_UNANCHORED, &match, 1);
    
    if (num_matches > 0) {
        // Match found, do something with the match
        
        // Print the matched string
        std::cout << "Matched string: " << std::string(match.data, match.length) << std::endl;
    }
    
    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    
    return 0;
}