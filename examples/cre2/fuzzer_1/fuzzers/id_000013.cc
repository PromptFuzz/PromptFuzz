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
//<ID> 1305
//<Prompt> ["cre2_opt_new","cre2_set_new","cre2_opt_set_one_line","cre2_new","cre2_delete","cre2_opt_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_opt_set_dot_nl","cre2_program_size","cre2_opt_delete","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"]
/*<Combination>: [cre2_options_t *cre2_opt_new(),
    cre2_set *cre2_set_new(cre2_options_t * opt, cre2_anchor_t anchor),
    void cre2_opt_set_one_line(cre2_options_t * opt, int flag),
    cre2_regexp_t *cre2_new(const char * pattern, int pattern_len, const cre2_options_t * opt),
    void cre2_delete(cre2_regexp_t * re),
    int cre2_opt_dot_nl(cre2_options_t * opt),
    cre2_regexp_t *cre2_new(const char * pattern, int pattern_len, const cre2_options_t * opt),
    cre2_named_groups_iter_t *cre2_named_groups_iter_new(const cre2_regexp_t * re),
    bool cre2_named_groups_iter_next(cre2_named_groups_iter_t * iter, const char ** name, int * index),
    void cre2_opt_set_dot_nl(cre2_options_t * opt, int flag),
    int cre2_program_size(const cre2_regexp_t * re),
    void cre2_opt_delete(cre2_options_t * opt),
    void cre2_set_delete(cre2_set * set),
    void cre2_delete(cre2_regexp_t * re),
    void cre2_named_groups_iter_delete(cre2_named_groups_iter_t * iter)
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["cre2_opt_new","cre2_set_new","cre2_opt_set_one_line","cre2_opt_set_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_program_size","cre2_opt_delete","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"],"critical_calls":["cre2_opt_new","cre2_set_new","cre2_opt_set_one_line","cre2_opt_set_dot_nl","cre2_new","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_program_size","cre2_opt_delete","cre2_set_delete","cre2_delete","cre2_named_groups_iter_delete"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Create options
    cre2_options_t *opt = cre2_opt_new();

    // Create a set
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Set options
    cre2_opt_set_one_line(opt, fuzz_int32_t_1);
    cre2_opt_set_dot_nl(opt, fuzz_int32_t_2);

    // Create a regular expression
    cre2_regexp_t *re = cre2_new(reinterpret_cast<const char *>(data), size, opt);

    // Create a named groups iterator
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);

    // Iterate over named groups
    const char *name;
    int index;
    while (cre2_named_groups_iter_next(iter, &name, &index)) {
        // Do something with named groups
    }

    // Get program size
    int program_size = cre2_program_size(re);

    // Delete resources
    cre2_opt_delete(opt);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_named_groups_iter_delete(iter);

    return 0;
}