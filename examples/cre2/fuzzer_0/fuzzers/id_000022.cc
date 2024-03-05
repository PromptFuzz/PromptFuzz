#include "FDSan.h"
#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2086
//<Prompt> ["cre2_full_match","cre2_global_replace","cre2_replace_re","cre2_full_match_re","cre2_extract_re","cre2_set_match","cre2_easy_match","cre2_extract","cre2_replace"]
/*<Combination>: [int cre2_full_match(const char * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_global_replace(const char * pattern, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_replace_re(cre2_regexp_t * rex, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_full_match_re(cre2_regexp_t * , const cre2_string_t * , cre2_string_t * , int ),
    int cre2_extract_re(cre2_regexp_t * rex, cre2_string_t * text, cre2_string_t * rewrite, cre2_string_t * target),
    size_t cre2_set_match(cre2_set * set, const char * text, size_t text_len, int * match, size_t match_len),
    int cre2_easy_match(const char * pattern, int pattern_len, const char * text, int text_len, cre2_string_t * match, int nmatch),
    int cre2_extract(const char * pattern, cre2_string_t * text, cre2_string_t * rewrite, cre2_string_t * target),
    int cre2_replace(const char * pattern, cre2_string_t * text_and_target, cre2_string_t * rewrite)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_log_errors","cre2_new","cre2_error_code","cre2_opt_delete","cre2_delete","cre2_full_match_re","cre2_opt_delete","cre2_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_posix_syntax","cre2_opt_set_log_errors","cre2_new","cre2_error_code","cre2_full_match_re","cre2_opt_delete","cre2_delete"],"visited":0}
/*Here is the implementation of the fuzz driver using cre2 library:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a regular expression pattern
    const char *pattern = "abc";

    // Create the options for cre2
    cre2_options_t *opt = cre2_opt_new();

    // Initialize the options
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_log_errors(opt, 0);

    // Compile the pattern
    cre2_regexp_t *re = cre2_new(pattern, strlen(pattern), opt);

    // Check if the regular expression is valid
    if (cre2_error_code(re) == CRE2_ERROR_BAD_ESCAPE) {
        // Invalid regular expression, return
        cre2_opt_delete(opt);
        cre2_delete(re);
        return 0;
    }

    // Create a cre2_string_t to store the input data
    cre2_string_t input;
    input.data = reinterpret_cast<const char*>(data);
    input.length = size;

    // Create a cre2_string_t to store the matched string
    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    // Perform the full match of the input data with the regular expression
    int result = cre2_full_match_re(re, &input, &match, 0);

    // Print the matched string if the match is successful
    if (result == 1) {
        printf("Matched: %.*s\n", match.length, match.data);
    }

    // Clean up allocated resources
    cre2_opt_delete(opt);
    cre2_delete(re);

    return 0;
}