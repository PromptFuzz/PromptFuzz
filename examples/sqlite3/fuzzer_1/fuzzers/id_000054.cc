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
//<ID> 1380
//<Prompt> ["sqlite3_libversion_number","sqlite3_profile","sqlite3_auto_extension","sqlite3_blob_open","sqlite3_blob_write","sqlite3_db_status","sqlite3_enable_load_extension","sqlite3_interrupt"]
/*<Combination>: [int sqlite3_libversion_number(),
    void *sqlite3_profile(sqlite3 * , void (*)(void *, const char *, sqlite3_uint64) xProfile, void * ),
    int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_blob_open(sqlite3 * , const char * zDb, const char * zTable, const char * zColumn, sqlite3_int64 iRow, int flags, sqlite3_blob ** ppBlob),
    int sqlite3_blob_write(sqlite3_blob * , const void * z, int n, int iOffset),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    int sqlite3_enable_load_extension(sqlite3 * db, int onoff),
    void sqlite3_interrupt(sqlite3 * )
*/
//<score> 1.6, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":10,"density":8,"covered_api":["sqlite3_libversion_number","sqlite3_profile","sqlite3_auto_extension","sqlite3_blob_open","sqlite3_blob_write","sqlite3_db_status","sqlite3_enable_load_extension","sqlite3_interrupt"],"uncovered_api":[],"unique_branches":{"sqlite3_profile":[[178754,7,178754,19,1]]},"library_calls":["sqlite3_open","sqlite3_libversion_number","sqlite3_profile","sqlite3_auto_extension","sqlite3_blob_open","sqlite3_blob_write","sqlite3_db_status","sqlite3_enable_load_extension","sqlite3_interrupt","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<=28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Read the input data using the in_file
    // Write the output data using the out_file

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Use the file descriptors as needed

    // Call the sqlite3 APIs to achieve the desired event
    sqlite3_libversion_number();
    sqlite3_profile(db, NULL, NULL);
    sqlite3_auto_extension(NULL);
    sqlite3_blob *pBlob;
    sqlite3_blob_open(db, "dbname", "tablename", "columnname", fuzz_int64_t_1, fuzz_int32_t_2, &pBlob);
    sqlite3_blob_write(pBlob, NULL, 0, fuzz_int32_t_3);
    int cur, hiwtr;
    sqlite3_db_status(db, fuzz_int32_t_4, &cur, &hiwtr, fuzz_int32_t_5);
    sqlite3_enable_load_extension(db, fuzz_int32_t_6);
    sqlite3_interrupt(db);

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