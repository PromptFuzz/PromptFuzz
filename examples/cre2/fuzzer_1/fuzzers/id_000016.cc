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
//<ID> 1507
//<Prompt> ["cre2_match","cre2_replace","cre2_extract","cre2_check_rewrite_string","cre2_quote_meta"]
/*<Combination>: [int cre2_match(const cre2_regexp_t * re, const char * text, int textlen, int startpos, int endpos, cre2_anchor_t anchor, cre2_string_t * match, int nmatch),
    int cre2_replace(const char * pattern, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_extract(const char * pattern, cre2_string_t * text, cre2_string_t * rewrite, cre2_string_t * target),
    int cre2_check_rewrite_string(cre2_regexp_t * rex, cre2_string_t * rewrite, cre2_string_t * errmsg),
    int cre2_quote_meta(cre2_string_t * quoted, cre2_string_t * original)
*/
//<score> 4, nr_unique_branch: 1
//<Quality> {"density":2,"unique_branches":{"cre2_replace":[[473,9,473,15,0,0,4,0]]},"library_calls":["cre2_new","cre2_match","cre2_replace","cre2_extract","cre2_check_rewrite_string","cre2_quote_meta","cre2_delete"],"critical_calls":["cre2_new","cre2_match","cre2_replace","cre2_extract","cre2_check_rewrite_string","cre2_quote_meta","cre2_delete"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Create a string for output file name
    const char *output_file = "output_file";
    
    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 5: Create a FILE * variable to write output data
    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Define the event that needs to be achieved
    int event = 0;
    
    // Step 2: Use cre2 APIs to achieve the event
    cre2_regexp_t *rex = cre2_new(fuzz_str_1, static_cast<int>(fuzz_str_sz_1), NULL);
    if (rex) {
        char error[256];
        cre2_string_t text_and_target;
        text_and_target.data = nullptr;
        text_and_target.length = 0;
        
        cre2_string_t rewrite;
        rewrite.data = nullptr;
        rewrite.length = 0;
        
        cre2_string_t target;
        target.data = nullptr;
        target.length = 0;
        
        cre2_string_t errmsg;
        errmsg.data = nullptr;
        errmsg.length = 0;
        
        cre2_string_t quoted;
        quoted.data = nullptr;
        quoted.length = 0;
        
        event += cre2_match(rex, nullptr, 0, fuzz_int32_t_2, fuzz_int32_t_3, CRE2_ANCHOR_START, nullptr, 0);
        event += cre2_replace(fuzz_str_4, &text_and_target, &rewrite);
        event += cre2_extract(fuzz_str_5, &text_and_target, &rewrite, &target);
        event += cre2_check_rewrite_string(rex, &rewrite, &errmsg);
        event += cre2_quote_meta(&quoted, &rewrite);
        
        cre2_delete(rex);
        
        // Step 4: Release resources before return
        if (text_and_target.data) {
            free((void *)text_and_target.data);
        }
        if (rewrite.data) {
            free((void *)rewrite.data);
        }
        if (target.data) {
            free((void *)target.data);
        }
        if (errmsg.data) {
            free((void *)errmsg.data);
        }
        if (quoted.data) {
            free((void *)quoted.data);
        }
    }
    
    // Step 7: Release resources before return
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return event;
}