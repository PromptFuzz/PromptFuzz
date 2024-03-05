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
//<ID> 2134
//<Prompt> ["sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_data_count","sqlite3_vmprintf","sqlite3_mutex_notheld"]
/*<Combination>: [const void *sqlite3_column_table_name16(sqlite3_stmt * , int ),
    int sqlite3_value_frombind(sqlite3_value * ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    int sqlite3_mutex_notheld(sqlite3_mutex * )
*/
//<score> 1, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":11,"density":11,"covered_api":["sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_data_count","sqlite3_vmprintf","sqlite3_mutex_notheld"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_data_count","sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_column_value","sqlite3_vmprintf","sqlite3_mutex_notheld","sqlite3_db_mutex","sqlite3_finalize","sqlite3_close"]}
/*Sure! Here is the step-by-step implementation of the C++ program that follows your instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* f_data, size_t f_size) {
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




    // Step 4: Create input and output file pointers
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    if (in_file == nullptr || out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Call sqlite3 APIs
    sqlite3_stmt *stmt;
    sqlite3 *db;
    char *errorMsg;

    // Open database
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

    // Prepare statement
    const char *sql = "SELECT * FROM books;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // Step 3: Call sqlite3_data_count API
    int count = sqlite3_data_count(stmt);

    // Step 1: Call sqlite3_column_table_name16 API
    const void *tableName = sqlite3_column_table_name16(stmt, fuzz_int32_t_1);

    // Step 1: Call sqlite3_value_frombind API
    int fromBind = sqlite3_value_frombind(sqlite3_column_value(stmt, fuzz_int32_t_2));

    // Step 2: Call sqlite3_vmprintf API
    char *formattedSql = sqlite3_vmprintf(sql, NULL);

    // Step 5: Call sqlite3_mutex_notheld API
    int isMutexNotHeld = sqlite3_mutex_notheld(sqlite3_db_mutex(db));

    // Step 6: Use file name directly
    const char *inputFileName = "input_file";
    const char *outputFileName = "output_file";

    // Step 7: Release resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(formattedSql);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}