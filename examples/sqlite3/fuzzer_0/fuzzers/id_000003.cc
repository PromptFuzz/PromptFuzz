#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 182
//<Prompt> ["sqlite3_snapshot_recover","sqlite3_system_errno","sqlite3_initialize","sqlite3_filename_wal","sqlite3_vtab_config"]
/*<Combination>: [int sqlite3_snapshot_recover(sqlite3 * db, const char * zDb),
    int sqlite3_system_errno(sqlite3 * ),
    int sqlite3_initialize(),
    const char *sqlite3_filename_wal(sqlite3_filename ),
    int sqlite3_vtab_config(sqlite3 * , int op)
*/
//<score> 3.3600001, nr_unique_branch: 6, p_cov: 0.6
//<Quality> {"diversity":10,"density":8,"covered_api":["sqlite3_snapshot_recover","sqlite3_system_errno","sqlite3_vtab_config"],"uncovered_api":["sqlite3_initialize","sqlite3_filename_wal"],"unique_branches":{"sqlite3PagerSnapshotRecover":[[63955,7,63955,19,1]],"sqlite3_snapshot_recover":[[181380,7,181380,32,1],[181387,7,181387,13,0],[181389,9,181389,51,0],[181391,11,181391,24,0],[181391,11,181391,24,1]]},"library_calls":["sqlite3_open_v2","sqlite3_snapshot_recover","sqlite3_vtab_config","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create file to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Step 6: Pass file name as string
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Step 1: Use sqlite3 APIs to achieve desired event
    sqlite3 *db;
    int rc = sqlite3_open_v2(input_file, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        // Step 3: Consume input data
        int error_code = sqlite3_system_errno(db);
        printf("Failed to open database: %s (error code: %d)\n", sqlite3_errmsg(db), error_code);
        
        // Step 7: Release resources
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    const char *zDb = "main";
    rc = sqlite3_snapshot_recover(db, zDb);
    if (rc != SQLITE_OK) {
        // Step 1: Use another sqlite3 API
        printf("Failed to recover snapshot: %s\n", sqlite3_errmsg(db));
        
        // Step 7: Release resources
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor
    int input_fd = fuzz_fileno(in_file);
    
    // Do some operations with input_fd
    
    // Step 2: Consume input data
    fseek(in_file, 0, SEEK_END);
    size_t file_size = ftell(in_file);
    printf("Input file size: %zu\n", file_size);
    
    // Step 4: Create file to write output data
    FILE *out_file = fopen(output_file, "wb");
    if (out_file == nullptr) {
        // Step 3: Consume input data
        printf("Failed to open output file\n");
        
        // Step 7: Release resources
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }
    
    // Step 5: Get file descriptor
    int output_fd = fuzz_fileno(out_file);
    
    // Do some operations with output_fd
    
    // Step 1: Use another sqlite3 API
    int op = SQLITE_CONFIG_SINGLETHREAD;
    sqlite3_vtab_config(db, op);
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}