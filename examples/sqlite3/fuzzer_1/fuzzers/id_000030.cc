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
//<ID> 913
//<Prompt> ["sqlite3_open","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    int sqlite3_keyword_name(int , const char ** , int * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * )
*/
//<score> 6.75, nr_unique_branch: 8, p_cov: 1
//<Quality> {"diversity":8,"density":6,"covered_api":["sqlite3_open","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt"],"uncovered_api":[],"unique_branches":{"walIndexRecover":[[65522,13,65522,22,1]],"walCheckpoint":[[66304,16,66304,32,1],[66322,11,66322,24,1],[66352,7,66352,20,1],[13686,32,13686,71,0]],"sqlite3WalCheckpoint":[[68288,11,68288,24,1],[68288,28,68288,43,1]],"sqlite3WalClose":[[66529,11,66529,24,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    FILE* in_file = fmemopen((void*)data, size, "rb");

    // Open a new sqlite3 database
    sqlite3* db;
    sqlite3_open(":memory:", &db);

    // Read and execute the SQL statements from the input file
    char sql[1024];
    while (fgets(sql, sizeof(sql), in_file)) {
        sqlite3_exec(db, sql, NULL, 0, NULL);
    }

    // Get the status of the database
    int cur = 0;
    int hiwtr = 0;
    sqlite3_db_status(db, fuzz_int32_t_1, &cur, &hiwtr, fuzz_int32_t_2);

    // Get the list of keywords
    const char* keyword;
    int len;
    for (int i = 0; i < 100; i++) {
        sqlite3_keyword_name(i, &keyword, &len);
    }

    // Set memory alarm callback
    void (*callback)(void*, sqlite3_int64, int);
    void* arg;
    sqlite3_memory_alarm(callback, arg, fuzz_int64_t_3);

    // Get the filename of the database
    const char* filename = sqlite3_db_filename(db, "main");

    // Interrupt the currently executing SQL statement
    sqlite3_interrupt(db);

    // Clean up resources
    assert_file_closed(&in_file);;
    sqlite3_close(db);

    assert_file_closed(&in_file);
	return 0;
}