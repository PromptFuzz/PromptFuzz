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
//<ID> 591
//<Prompt> ["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_enable_load_extension","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"]
/*<Combination>: [int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_blob_write(sqlite3_blob * , const void * z, int n, int iOffset),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64 N),
    int sqlite3_enable_load_extension(sqlite3 * db, int onoff),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    int sqlite3_mutex_held(sqlite3_mutex * )
*/
//<score> 8.571428, nr_unique_branch: 11, p_cov: 1
//<Quality> {"diversity":14,"density":10,"covered_api":["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_enable_load_extension","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"],"uncovered_api":[],"unique_branches":{"sqlite3_blob_close":[[101182,7,101182,8,1]],"sqlite3_enable_load_extension":[[135685,7,135685,12,0]],"sqlite3_blob_open":[[100983,9,100983,13,1],[100987,9,100987,13,1],[100992,9,100992,13,1],[100997,9,100997,14,0],[100998,11,100998,25,0],[101159,7,101159,20,1],[101162,9,101162,14,0],[101162,18,101162,30,1],[101165,32,101165,36,0]]},"library_calls":["sqlite3_open","sqlite3_auto_extension","sqlite3_blob_open","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_enable_load_extension","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held","sqlite3_blob_close","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<=60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_9, fdp);
	//fuzzer shim end}




    // Create FILE pointers for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize SQLite3 database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Handle SQLite3 open error
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

    // Register custom extension
    sqlite3_auto_extension([](){
        // Custom extension logic
    });

    // Create a blob in the database
    sqlite3_blob *blob;
    sqlite3_blob_open(db, "main", "table", "column", fuzz_int64_t_1, fuzz_int32_t_2, &blob);

    // Write data to the blob
    const char *blob_data = "Hello, World!";
    sqlite3_blob_write(blob, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), fuzz_int32_t_4);

    // Get SQLite3 database status
    int cur_status, hiwtr_status;
    sqlite3_db_status(db, fuzz_int32_t_5, &cur_status, &hiwtr_status, fuzz_int32_t_6);

    // Set hard heap limit
    sqlite3_hard_heap_limit64(fuzz_int64_t_7);

    // Enable load extension
    sqlite3_enable_load_extension(db, fuzz_int32_t_8);

    // Get database file name
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");

    // Interrupt the ongoing SQLite3 operation
    sqlite3_interrupt(db);

    // Set memory alarm
    sqlite3_memory_alarm([](void *, sqlite3_int64, int){
        // Memory alarm logic
    }, nullptr, fuzz_int64_t_9);

    // Check if mutex is held
    sqlite3_mutex_held(nullptr);

    // Clean up
    sqlite3_blob_close(blob);
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