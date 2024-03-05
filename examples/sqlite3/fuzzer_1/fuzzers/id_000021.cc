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
//<ID> 662
//<Prompt> ["sqlite3_error_offset","sqlite3_value_text16","sqlite3_column_database_name16","sqlite3_stmt_status","sqlite3_mutex_enter"]
/*<Combination>: [int sqlite3_error_offset(sqlite3 * db),
    const void *sqlite3_value_text16(sqlite3_value * ),
    const void *sqlite3_column_database_name16(sqlite3_stmt * , int ),
    int sqlite3_stmt_status(sqlite3_stmt * , int op, int resetFlg),
    void sqlite3_mutex_enter(sqlite3_mutex * )
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":10,"density":10,"covered_api":["sqlite3_error_offset","sqlite3_value_text16","sqlite3_column_database_name16","sqlite3_stmt_status"],"uncovered_api":["sqlite3_mutex_enter"],"unique_branches":{"sqlite3_stmt_status":[[91164,7,91164,13,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_text16","sqlite3_stmt_status","sqlite3_error_offset","sqlite3_value_text16","sqlite3_column_value","sqlite3_column_database_name16","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<=36) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Step 4: Open input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        // Error handling
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Call sqlite3 APIs
    sqlite3 *db;
    sqlite3_open(":memory:", &db);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM table", -1, &stmt, nullptr);
    sqlite3_bind_text16(stmt, fuzz_int32_t_2, fuzz_str_1, fuzz_int32_t_3, SQLITE_STATIC);

    int status = sqlite3_stmt_status(stmt, fuzz_int32_t_4, fuzz_int32_t_5);
    int error_offset = sqlite3_error_offset(db);

    // Step 3: Consume input data
    fread(&status, sizeof(int), 1, in_file);
    fread(&error_offset, sizeof(int), 1, in_file);

    // Step 6: Use file names
    const char *in_filename = "input_file";
    const char *out_filename = "output_file";

    // Step 1: Call other sqlite3 APIs
    const void *value_text16 = sqlite3_value_text16(sqlite3_column_value(stmt, fuzz_int32_t_6));
    const void *column_db_name16 = sqlite3_column_database_name16(stmt, fuzz_int32_t_7);

    // Perform operations with the consumed input data and other APIs

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}