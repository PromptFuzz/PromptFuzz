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
//<ID> 303
//<Prompt> ["cre2_opt_never_nl","cre2_quote_meta","cre2_set_add","cre2_set_add_simple","cre2_opt_literal","cre2_opt_dot_nl","cre2_program_size","cre2_named_groups_iter_next","cre2_opt_set_dot_nl"]
/*<Combination>: [int cre2_opt_never_nl(cre2_options_t * opt),
    int cre2_quote_meta(cre2_string_t * quoted, cre2_string_t * original),
    int cre2_set_add(cre2_set * set, const char * pattern, size_t pattern_len, char * error, size_t error_len),
    int cre2_set_add_simple(cre2_set * set, const char * pattern),
    int cre2_opt_literal(cre2_options_t * opt),
    int cre2_opt_dot_nl(cre2_options_t * opt),
    int cre2_program_size(const cre2_regexp_t * re),
    bool cre2_named_groups_iter_next(cre2_named_groups_iter_t * iter, const char ** name, int * index),
    void cre2_opt_set_dot_nl(cre2_options_t * opt, int flag)
*/
//<score> 22.5, nr_unique_branch: 3
//<Quality> {"density":15,"unique_branches":{"cre2_set_add":[[696,7,696,22,0,0,4,1],[696,26,696,42,0,0,4,0]],"_ZN3re23RE23SetD2Ev":[[29,22,29,38,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_set_new","cre2_set_add","cre2_set_add_simple","cre2_opt_literal","cre2_opt_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_opt_set_dot_nl","cre2_program_size","cre2_opt_delete","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"],"critical_calls":["cre2_opt_new","cre2_set_new","cre2_set_add","cre2_set_add_simple","cre2_opt_literal","cre2_opt_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_opt_set_dot_nl","cre2_program_size","cre2_opt_delete","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 5: Get the file descriptor for reading
    int fd = fuzz_fileno(in_file);
    
    // Step 6: Get the file name for reading
    const char *file_name = "input_file";
    
    // Step 1: Use cre2 APIs to achieve the desired event
    cre2_options_t *opt = cre2_opt_new();
    cre2_string_t quoted;
    cre2_string_t original;
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);
    char error[100];
    size_t error_len;
    cre2_set_add(set, fuzz_str_1, static_cast<size_t>(fuzz_str_sz_1), error, error_len);
    cre2_set_add_simple(set, fuzz_str_2);
    cre2_opt_literal(opt);
    cre2_opt_dot_nl(opt);
    const cre2_regexp_t *re = cre2_new(fuzz_str_3, static_cast<int>(fuzz_str_sz_3), opt);
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    const char *name;
    int index;
    cre2_named_groups_iter_next(iter, &name, &index);
    cre2_opt_set_dot_nl(opt, fuzz_int32_t_4);
    int program_size = cre2_program_size(re);
    
    // Step 7: Release resources
    cre2_opt_delete(opt);
    cre2_set_delete(set);
    cre2_delete((cre2_regexp_t *)re);
    cre2_named_groups_iter_delete(iter);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}