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
//<ID> 561
//<Prompt> ["sqlite3_auto_extension","sqlite3_db_status","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"]
/*<Combination>: [int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    int sqlite3_mutex_held(sqlite3_mutex * )
*/
//<score> 9.333334, nr_unique_branch: 11, p_cov: 1
//<Quality> {"diversity":9,"density":7,"covered_api":["sqlite3_auto_extension","sqlite3_db_status","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"],"uncovered_api":[],"unique_branches":{"sqlite3SafetyCheckOk":[[35808,7,35808,36,0],[35809,9,35809,39,0],[13906,27,13906,28,1]],"sqlite3SafetyCheckSickOrOk":[[35821,7,35821,36,1]],"sqlite3_interrupt":[[178330,8,178330,13,1],[178330,17,178330,52,0]],"sqlite3_db_mutex":[[177378,7,177378,32,0]],"sqlite3LeaveMutexAndCloseZombie":[[177943,7,177943,30,1]],"sqlite3AutoLoadExtensions":[[135853,18,135853,55,0]],"openDatabase":[[180007,9,180007,22,0],[180048,13,180048,26,0]]},"library_calls":["sqlite3_auto_extension","sqlite3_open","sqlite3_db_status","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt","sqlite3_db_mutex","sqlite3_mutex_held","sqlite3_close"]}
/*Here is the C++ program that achieves the event by using the sqlite3 APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    sqlite3_auto_extension([]() {}); // sqlite3_auto_extension
    sqlite3 *db;
    sqlite3_open(":memory:", &db);

    // Read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);

    // Write the output data
    FILE *out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);

    // Use the input and output file descriptors
    int db_status_cur, db_status_hiwtr;
    sqlite3_db_status(db, fuzz_int32_t_1, &db_status_cur, &db_status_hiwtr, fuzz_int32_t_2);
    sqlite3_memory_alarm([](void *, sqlite3_int64, int) {}, nullptr, fuzz_int64_t_3);

    // Use the input and output file names
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    printf("Database filename: %s\n", db_filename);

    // Use the interrupt API
    sqlite3_interrupt(db);

    // Use the mutex API
    sqlite3_mutex *mutex = sqlite3_db_mutex(db);
    sqlite3_mutex_held(mutex);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}