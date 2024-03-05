#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1489
//<Prompt> ["sqlite3_str_value","sqlite3_data_count","sqlite3_auto_extension","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [char *sqlite3_str_value(sqlite3_str * ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 1.3636364, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":11,"density":5,"covered_api":["sqlite3_str_value","sqlite3_data_count","sqlite3_auto_extension","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_finalize"],"uncovered_api":[],"unique_branches":{"sqlite3_data_count":[[90372,7,90372,13,0]],"sqlite3_str_value":[[31600,15,31600,26,0]]},"library_calls":["sqlite3_initialize","sqlite3_str_new","sqlite3_str_value","sqlite3_prepare_v3","sqlite3_data_count","sqlite3_auto_extension","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_finalize","sqlite3_str_finish","sqlite3_shutdown"]}
/*Here is the C++ program that achieves the given event by using the sqlite3 library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file variable
    FILE *in_file = fmemopen((void*)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Get input file name
    const char *input_file = "input_file";

    // Step 7: Get input file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Initialize sqlite3
    sqlite3_initialize();
    
    // Step 3: Consume input data and pass it to sqlite3 APIs
    sqlite3_str *str = sqlite3_str_new(nullptr);
    char *result = sqlite3_str_value(str); // Step 1: API call
    
    sqlite3_stmt *stmt;
    sqlite3_prepare_v3(nullptr, input_file, -1, 0, &stmt, nullptr);
    int count = sqlite3_data_count(stmt); // Step 1: API call
    
    sqlite3_auto_extension(nullptr); // Step 1: API call
    
    int paramCount = sqlite3_bind_parameter_count(stmt); // Step 1: API call
    
    int stepResult = sqlite3_step(stmt); // Step 1: API call
    
    sqlite3_finalize(stmt); // Step 1: API call
    
    // Step 5: Get output file descriptor
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);

    // Step 4: Close input file
    assert_file_closed(&in_file);;

    // Step 7: Close output file
    assert_file_closed(&out_file);;

    // Release allocated resources
    sqlite3_str_finish(str);
    sqlite3_shutdown();

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}