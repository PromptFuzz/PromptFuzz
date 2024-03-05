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
//<ID> 1303
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
//<score> 91, nr_unique_branch: 10
//<Quality> {"density":13,"unique_branches":{"_ZN3re23DFA14RunStateOnByteEPNS0_5StateEi":[[1100,18,1100,43,0,0,4,0]],"_ZN3re23DFA18WorkqToCachedStateEPNS0_5WorkqES2_j":[[723,7,723,17,0,0,4,0],[725,43,725,57,0,0,4,0],[725,43,725,57,0,0,4,1],[728,11,728,37,0,0,4,0],[728,11,728,37,0,0,4,1]],"_ZN3re214SimplifyWalker14SimplifyRepeatEPNS_6RegexpEiiNS1_10ParseFlagsE":[[606,8,606,65,0,0,4,0]],"_ZN3re26Regexp10ParseState10PushRegexpEPS0_":[[243,38,243,54,0,0,4,1]],"_ZN3re28Compiler9PostVisitEPNS_6RegexpENS_4FragES3_PS3_i":[[858,11,858,38,0,0,4,0]],"_ZN3re28Compiler10CompileSetEPNS_6RegexpENS_3RE26AnchorEl":[[1250,7,1250,17,0,0,4,0]]},"library_calls":["cre2_opt_new","cre2_opt_set_one_line","cre2_new","cre2_program_size","cre2_set_new","cre2_set_add","cre2_set_compile","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_named_groups_iter_delete","cre2_set_delete","cre2_delete","cre2_opt_delete"],"critical_calls":["cre2_opt_new","cre2_opt_set_one_line","cre2_new","cre2_program_size","cre2_set_new","cre2_set_add","cre2_set_compile","cre2_named_groups_iter_new","cre2_named_groups_iter_next","cre2_named_groups_iter_delete","cre2_set_delete","cre2_delete","cre2_opt_delete"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Create options
    cre2_options_t *opt = cre2_opt_new();
    // Set option to treat input as a single line
    cre2_opt_set_one_line(opt, fuzz_int32_t_1);

    // Create a regular expression object
    const char *pattern = (const char *)data;
    int pattern_len = size;
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re != NULL) {
        // Get the program size of the regular expression
        int program_size = cre2_program_size(re);

        // Create a set object with the regular expression
        cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_BOTH);
        cre2_set_add(set, pattern, pattern_len, NULL, 0);
        cre2_set_compile(set);

        // Create a named groups iterator for the regular expression
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);

        // Iterate over the named groups
        const char *name;
        int index;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // Do something with the named group
        }

        // Free resources
        cre2_named_groups_iter_delete(iter);
        cre2_set_delete(set);
        cre2_delete(re);
    }

    // Free resources
    cre2_opt_delete(opt);

    return 0;
}