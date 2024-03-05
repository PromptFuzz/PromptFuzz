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
//<ID> 884
//<Prompt> ["sqlite3_open","sqlite3_db_status","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_str_appendf","sqlite3_interrupt"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_str_appendf(sqlite3_str * , const char * zFormat),
    void sqlite3_interrupt(sqlite3 * )
*/
//<score> 3.6, nr_unique_branch: 3, p_cov: 1
//<Quality> {"diversity":10,"density":9,"covered_api":["sqlite3_open","sqlite3_db_status","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_str_appendf","sqlite3_interrupt"],"uncovered_api":[],"unique_branches":{"sqlite3_db_status":[[23595,11,23595,20,0],[23597,13,23597,14,1],[23605,13,23605,14,1]]},"library_calls":["sqlite3_open","sqlite3_db_status","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_finish","sqlite3_interrupt","sqlite3_close","sqlite3_free"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE pointer to read the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");

    // Step 5: Get the file descriptor of the input file
    int in_fd = fuzz_fileno(in_file);

    // Step 7: Close the input file and release resources
    assert_file_closed(&in_file);;

    // Step 6: Directly use the string "input_file" as the file name
    const char* input_file = "input_file";

    // Step 1: Call sqlite3_open to open a SQLite database
    sqlite3* db;
    int rc = sqlite3_open(input_file, &db);
    if (rc != SQLITE_OK) {
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 1: Call sqlite3_db_status to get the status of the database
    int cur, hiwtr;
    rc = sqlite3_db_status(db, fuzz_int32_t_1, &cur, &hiwtr, fuzz_int32_t_2);

    // Step 1: Call sqlite3_memory_alarm to set a memory alarm callback
    sqlite3_memory_alarm(nullptr, 0, fuzz_int64_t_3);

    // Step 1: Call sqlite3_db_filename to get the filename of a database
    const char* db_filename = sqlite3_db_filename(db, "main");

    // Step 1: Call sqlite3_str_appendf to append a formatted string to a string buffer
    sqlite3_str* str = sqlite3_str_new(nullptr);
    sqlite3_str_appendf(str, "Database filename: %s\n", db_filename);
    const char* str_value = sqlite3_str_finish(str);

    // Step 1: Call sqlite3_interrupt to interrupt a database operation
    sqlite3_interrupt(db);

    // Step 7: Close the SQLite database and release resources
    sqlite3_close(db);

    // Step 7: Free the string buffer
    sqlite3_free((void*)str_value);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}