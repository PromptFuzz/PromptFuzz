#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 556
//<Prompt> ["sqlite3_db_status","sqlite3_db_filename","sqlite3_interrupt","sqlite3_mutex_enter","sqlite3_memory_alarm","sqlite3_mutex_held"]
/*<Combination>: [int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * ),
    void sqlite3_mutex_enter(sqlite3_mutex * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    int sqlite3_mutex_held(sqlite3_mutex * )
*/
//<score> 2.1875, nr_unique_branch: 2, p_cov: 0.8333333
//<Quality> {"diversity":8,"density":7,"covered_api":["sqlite3_db_status","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"],"uncovered_api":["sqlite3_mutex_enter"],"unique_branches":{"sqlite3DbNameToBtree":[[181224,10,181224,15,0]],"sqlite3_db_filename":[[181258,10,181258,13,1]]},"library_calls":["sqlite3_open","sqlite3_db_mutex","sqlite3_db_status","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    sqlite3 *db;
    sqlite3_mutex *mutex;
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    const char *db_name = "database.db";
    int cur, hiwtr;
    
    // Open the database
    sqlite3_open(db_name, &db);

    // Acquire the database mutex
    mutex = sqlite3_db_mutex(db);

    // Get the database status
    sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiwtr, 0);

    // Get the filename of the database
    sqlite3_filename db_filename = sqlite3_db_filename(db, db_name);

    // Interrupt any ongoing database operation
    sqlite3_interrupt(db);
  
    // Set memory alarm callback
    sqlite3_memory_alarm(NULL, NULL, 0);

    // Check if the mutex is held by the current thread
    int held = sqlite3_mutex_held(mutex);

    // Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}