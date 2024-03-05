#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 588
//<Prompt> ["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"]
/*<Combination>: [int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_blob_write(sqlite3_blob * , const void * z, int n, int iOffset),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    sqlite3_int64 sqlite3_hard_heap_limit64(sqlite3_int64 N),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    int sqlite3_mutex_held(sqlite3_mutex * )
*/
//<score> 0.44444445, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":9,"density":4,"covered_api":["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Call APIs with the input data
    sqlite3_auto_extension(NULL);
    sqlite3_blob_write(NULL, NULL, 0, 0);
    sqlite3 *db;
    sqlite3_db_status(db, 0, NULL, NULL, 0);
    sqlite3_int64 heap_limit = sqlite3_hard_heap_limit64(0);
    const char *db_filename = sqlite3_db_filename(db, NULL);
    sqlite3_interrupt(db);
    sqlite3_memory_alarm(NULL, NULL, 0);
    sqlite3_mutex_held(NULL);

    // Release resources
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}