#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 3070
//<Prompt> ["sqlite3_keyword_name","sqlite3_db_release_memory","sqlite3_create_window_function","sqlite3_busy_handler","sqlite3_trace_v2","sqlite3_db_filename"]
/*<Combination>: [int sqlite3_keyword_name(int , const char ** , int * ),
    int sqlite3_db_release_memory(sqlite3 * ),
    int sqlite3_create_window_function(sqlite3 * db, const char * zFunctionName, int nArg, int eTextRep, void * pApp, void (*)(sqlite3_context *, int, sqlite3_value **) xStep, void (*)(sqlite3_context *) xFinal, void (*)(sqlite3_context *) xValue, void (*)(sqlite3_context *, int, sqlite3_value **) xInverse, void (*)(void *) xDestroy),
    int sqlite3_busy_handler(sqlite3 * , int (*)(void *, int) , void * ),
    int sqlite3_trace_v2(sqlite3 * , unsigned int uMask, int (*)(unsigned int, void *, void *, void *) xCallback, void * pCtx),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName)
*/
//<score> 3.3333333, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":6,"density":5,"covered_api":["sqlite3_keyword_name","sqlite3_db_release_memory","sqlite3_create_window_function","sqlite3_busy_handler","sqlite3_trace_v2","sqlite3_db_filename"],"uncovered_api":[],"unique_branches":{"sqlite3_trace_v2":[[178713,7,178713,32,0]],"sqlite3_db_release_memory":[[177394,7,177394,32,0]],"sqlite3_busy_handler":[[178262,7,178262,32,0]]},"library_calls":["sqlite3_keyword_name","sqlite3_db_release_memory","sqlite3_create_window_function","sqlite3_busy_handler","sqlite3_trace_v2","sqlite3_db_filename"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=30) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 6: Define the name of the input file
    const char *input_file = "input_file";

    // Step 5: Obtain the file descriptor for reading
    int input_fd = fuzz_fileno(in_file);

    // Step 4: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");

    // Step 6: Define the name of the output file
    const char *output_file = "output_file";

    // Step 5: Obtain the file descriptor for writing
    int output_fd = fuzz_fileno(out_file);

    // Step 1: Call sqlite3_keyword_name API
    const char *keyword;
    int len;
    sqlite3_keyword_name(0, &keyword, &len);

    // Step 1: Call sqlite3_db_release_memory API
    sqlite3 *db;
    sqlite3_db_release_memory(db);

    // Step 1: Call sqlite3_create_window_function API
    sqlite3_create_window_function(db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    // Step 1: Call sqlite3_busy_handler API
    sqlite3_busy_handler(db, nullptr, nullptr);

    // Step 1: Call sqlite3_trace_v2 API
    sqlite3_trace_v2(db, fuzz_uint32_t_4, nullptr, nullptr);

    // Step 1: Call sqlite3_db_filename API
    sqlite3_filename filename = sqlite3_db_filename(db, "database");

    // Release all allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}