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
//<ID> 472
//<Prompt> ["sqlite3_bind_pointer","sqlite3_wal_autocheckpoint","sqlite3_limit","sqlite3_result_value","sqlite3_free"]
/*<Combination>: [int sqlite3_bind_pointer(sqlite3_stmt * , int , void * , const char * , void (*)(void *) ),
    int sqlite3_wal_autocheckpoint(sqlite3 * db, int N),
    int sqlite3_limit(sqlite3 * , int id, int newVal),
    void sqlite3_result_value(sqlite3_context * , sqlite3_value * ),
    void sqlite3_free(void * )
*/
//<score> 1.8000001, nr_unique_branch: 2, p_cov: 0.6
//<Quality> {"diversity":8,"density":8,"covered_api":["sqlite3_bind_pointer","sqlite3_wal_autocheckpoint","sqlite3_limit"],"uncovered_api":["sqlite3_result_value","sqlite3_free"],"unique_branches":{"sqlite3_bind_pointer":[[90878,7,90878,20,1],[90881,13,90881,24,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_pointer","sqlite3_wal_autocheckpoint","sqlite3_limit","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<=26) return 0;

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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE * variables for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for input and output
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Use sqlite3_bind_pointer to bind a custom pointer to a parameter
    sqlite3_stmt *stmt = nullptr;
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    const char *sql = "CREATE TABLE test (id INT, name TEXT);";
    rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    void *ptr = malloc(sizeof(int));
    int value = 123;
    memcpy(ptr, &value, sizeof(int));
    sqlite3_bind_pointer(stmt, fuzz_int32_t_2, ptr, fuzz_str_1, free);

    // Step 2: Use sqlite3_wal_autocheckpoint to set the WAL autocheckpoint interval
    sqlite3_wal_autocheckpoint(db, fuzz_int32_t_3);

    // Step 3: Use sqlite3_limit to set a limit on the maximum number of rows
    sqlite3_limit(db, fuzz_int32_t_4, fuzz_int32_t_5);

    // Step 6: Use file names as strings
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Perform other operations using the SQLite API

    // Step 7: Clean up and release resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}