#include "FDSan.h"
#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 243
//<Prompt> ["cre2_opt_word_boundary","cre2_global_replace_re","cre2_set_add","cre2_quote_meta","cre2_opt_set_literal","cre2_consume_re","cre2_find_and_consume_re"]
/*<Combination>: [int cre2_opt_word_boundary(cre2_options_t * opt),
    int cre2_global_replace_re(cre2_regexp_t * rex, cre2_string_t * text_and_target, cre2_string_t * rewrite),
    int cre2_set_add(cre2_set * set, const char * pattern, size_t pattern_len, char * error, size_t error_len),
    int cre2_quote_meta(cre2_string_t * quoted, cre2_string_t * original),
    void cre2_opt_set_literal(cre2_options_t * opt, int flag),
    int cre2_consume_re(cre2_regexp_t * , cre2_string_t * , cre2_string_t * , int ),
    int cre2_find_and_consume_re(cre2_regexp_t * , cre2_string_t * , cre2_string_t * , int )
*/
//<score> 12, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"cre2_find_and_consume_re":[[435,19,435,27,0,0,4,0],[435,19,435,27,0,0,4,1],[440,9,440,15,0,0,4,1]],"_ZNK3re23RE27Options10ParseFlagsEv":[[173,7,173,16,0,0,4,0],[191,7,191,22,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_opt_set_literal","cre2_opt_set_word_boundary","cre2_new","cre2_consume_re","cre2_find_and_consume_re","cre2_opt_delete","cre2_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_literal","cre2_opt_set_word_boundary","cre2_new","cre2_consume_re","cre2_find_and_consume_re","cre2_opt_delete","cre2_delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    cre2_options_t *opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    cre2_string_t text_and_target;
    memset(&text_and_target, 0, sizeof(text_and_target));
    text_and_target.data = "input_file";
    text_and_target.length = strlen(text_and_target.data);

    cre2_string_t rewrite;
    memset(&rewrite, 0, sizeof(rewrite));
    rewrite.data = "output_file";
    rewrite.length = strlen(rewrite.data);

    cre2_regexp_t *rex = cre2_new("pattern", strlen("pattern"), opt);

    cre2_consume_re(rex, &text_and_target, &rewrite, in_fd);
    cre2_find_and_consume_re(rex, &text_and_target, &rewrite, out_fd);

    // Release resources
    cre2_opt_delete(opt);
    cre2_delete(rex);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}