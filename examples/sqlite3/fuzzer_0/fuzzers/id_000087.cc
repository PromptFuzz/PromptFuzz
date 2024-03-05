#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2308
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_create_function_v2(sqlite3 * db, const char * zFunctionName, int nArg, int eTextRep, void * pApp, void (*)(sqlite3_context *, int, sqlite3_value **) xFunc, void (*)(sqlite3_context *, int, sqlite3_value **) xStep, void (*)(sqlite3_context *) xFinal, void (*)(void *) xDestroy),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    int sqlite3_create_collation16(sqlite3 * , const void * zName, int eTextRep, void * pArg, int (*)(void *, int, const void *, int, const void *) xCompare),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 0.33333334, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":6,"density":1,"covered_api":["sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_snprintf"],"uncovered_api":[],"unique_branches":{"createFunctionApi":[[178512,7,178512,32,0]]},"library_calls":["sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_snprintf"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Use the file name directly
    const char *input_file = "input_file";

    // Step 7: Release file resources
    assert_file_closed(&in_file);;

    // SQLite3 APIs using the input data
    sqlite3_int64 last_insert_rowid = sqlite3_last_insert_rowid(NULL);

    int ret_code = sqlite3_create_function_v2(NULL, "func", 1, SQLITE_UTF8, NULL, NULL, NULL, NULL, NULL);

    const void *blob_data = "blob_data";
    int blob_size = 9;
    int bind_blob_ret = sqlite3_bind_blob(NULL, 1, blob_data, blob_size, NULL);

    int keyword_count = sqlite3_keyword_count();

    int create_collation_ret = sqlite3_create_collation16(NULL, NULL, SQLITE_UTF8, NULL, NULL);

    char output[256];
    char *snprintf_ret = sqlite3_snprintf(256, output, "Hello %s", "world");

    // Step 4: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Use the file name directly
    const char *output_file = "output_file";

    // Step 7: Release file resources
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}