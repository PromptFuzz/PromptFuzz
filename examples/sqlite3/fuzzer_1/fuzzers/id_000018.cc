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
//<ID> 615
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
//<score> 1.0909091, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":6,"covered_api":["sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_enable_load_extension","sqlite3_db_filename","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held"],"uncovered_api":[],"unique_branches":{"sqlite3_db_status":[[23587,36,23587,47,0]]},"library_calls":["sqlite3_open","sqlite3_enable_load_extension","sqlite3_auto_extension","sqlite3_blob_write","sqlite3_db_status","sqlite3_hard_heap_limit64","sqlite3_interrupt","sqlite3_memory_alarm","sqlite3_mutex_held","sqlite3_db_filename","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<=32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_6, fdp);
	//fuzzer shim end}




  // Open input file
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Open output file
  FILE *out_file = fopen("output_file", "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptor for input file
  int in_fd = fuzz_fileno(in_file);

  // Get file descriptor for output file
  int out_fd = fuzz_fileno(out_file);

  // Initialize sqlite3
  sqlite3 *db;
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

  // Load sqlite3 extensions
  sqlite3_enable_load_extension(db, fuzz_int32_t_1);

  // Execute sqlite3 APIs
  sqlite3_auto_extension(nullptr);
  sqlite3_blob *blob;
  sqlite3_blob_write(blob, nullptr, 0, fuzz_int32_t_2);
  sqlite3_db_status(db, fuzz_int32_t_3, nullptr, nullptr, fuzz_int32_t_4);
  sqlite3_hard_heap_limit64(fuzz_int64_t_5);
  sqlite3_interrupt(db);
  sqlite3_memory_alarm(nullptr, nullptr, fuzz_int64_t_6);
  sqlite3_mutex_held(nullptr);

  // Get database file name
  sqlite3_filename db_file = sqlite3_db_filename(db, "main");
  fwrite(db_file, sizeof(char), strlen(db_file), out_file);

  // Release resources
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