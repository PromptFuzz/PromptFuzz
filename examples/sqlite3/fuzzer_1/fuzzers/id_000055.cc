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
//<ID> 1386
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
//<score> 0.20833333, nr_unique_branch: 0, p_cov: 0.25
//<Quality> {"diversity":6,"density":5,"covered_api":["sqlite3_libversion_number","sqlite3_enable_load_extension"],"uncovered_api":["sqlite3_profile","sqlite3_auto_extension","sqlite3_blob_open","sqlite3_blob_write","sqlite3_db_status","sqlite3_interrupt"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_libversion_number","sqlite3_enable_load_extension","sqlite3_enable_load_extension","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<=8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 1: Create a sqlite3 database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Step 2: Execute the input data as SQL queries
    FILE *in_file = fmemopen((void *)data, size, "rb");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while ((read = getline(&line, &len, in_file)) != -1) {
        char *sql = line;
        
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        
        rc = sqlite3_exec(db, sql, 0, 0, 0);
        if (rc != SQLITE_OK) {
            break;
        }
    }
    
    if (line) {
        free(line);
    }
    
    assert_file_closed(&in_file);;
    
    // Step 3: Use other sqlite3 APIs
    int libversion = sqlite3_libversion_number();
    void *profile_data;
    int enable_extension = sqlite3_enable_load_extension(db, fuzz_int32_t_1); // Enable loading extension
    int disable_extension = sqlite3_enable_load_extension(db, fuzz_int32_t_2); // Disable loading extension
    
    // Step 4: Output the result
    FILE *out_file = fopen("output_file", "wb");
    fprintf(out_file, "libversion: %d\n", libversion);
    fprintf(out_file, "enable_extension: %d\n", enable_extension);
    fprintf(out_file, "disable_extension: %d\n", disable_extension);
    assert_file_closed(&out_file);;
    
    // Step 5: Release resources
    sqlite3_close(db);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}