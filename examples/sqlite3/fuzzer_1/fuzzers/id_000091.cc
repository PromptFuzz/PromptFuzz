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
//<ID> 2346
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    int sqlite3_create_collation16(sqlite3 * , const void * zName, int eTextRep, void * pArg, int (*)(void *, int, const void *, int, const void *) xCompare),
    int sqlite3_get_autocommit(sqlite3 * ),
    int sqlite3_config(int ),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 0.42857146, nr_unique_branch: 2, p_cov: 1
//<Quality> {"diversity":7,"density":1,"covered_api":["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"],"uncovered_api":[],"unique_branches":{"sqlite3_config":[[177255,5,177255,12,0]],"sqlite3_get_autocommit":[[180250,7,180250,32,0]]},"library_calls":["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the name of the input file
    const char *input_file = "input_file";

    // Step 7: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the name of the output file
    const char *output_file = "output_file";

    // Step 2: Call the sqlite3 APIs
    sqlite3_int64 last_insert_rowid = sqlite3_last_insert_rowid(NULL);
    int bind_blob_result = sqlite3_bind_blob(NULL, fuzz_int32_t_1, NULL, 0, NULL);
    int keyword_count = sqlite3_keyword_count();
    int create_collation16_result = sqlite3_create_collation16(NULL, NULL, fuzz_int32_t_2, NULL, NULL);
    int autocommit = sqlite3_get_autocommit(NULL);
    int config_result = sqlite3_config(0);
    char buffer[100];
    sqlite3_snprintf(sizeof(buffer), buffer, "Test buffer: %s", "Hello World");

    // Step 7: Write the output data to the file
    fwrite(buffer, sizeof(char), strlen(buffer), out_file);

    // Step 5: Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 4: Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Step 7: Release all allocated resources
    remove(input_file);
    remove(output_file);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}