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
//<ID> 1805
//<Prompt> ["sqlite3_drop_modules","sqlite3_vmprintf","sqlite3_backup_init","sqlite3_vtab_on_conflict","sqlite3_close_v2"]
/*<Combination>: [int sqlite3_drop_modules(sqlite3 * db, const char ** azKeep),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    sqlite3_backup *sqlite3_backup_init(sqlite3 * pDest, const char * zDestName, sqlite3 * pSource, const char * zSourceName),
    int sqlite3_vtab_on_conflict(sqlite3 * ),
    int sqlite3_close_v2(sqlite3 * )
*/
//<score> 2.4, nr_unique_branch: 8, p_cov: 0.4
//<Quality> {"diversity":9,"density":6,"covered_api":["sqlite3_drop_modules","sqlite3_close_v2"],"uncovered_api":["sqlite3_vmprintf","sqlite3_backup_init","sqlite3_vtab_on_conflict"],"unique_branches":{"sqlite3_drop_modules":[[152863,7,152863,32,1],[152865,44,152865,49,0],[152865,44,152865,49,1],[152868,9,152868,16,0],[152870,17,152870,31,1],[152871,11,152871,25,1]],"sqlite3VtabCreateModule":[[152789,7,152789,11,0],[152790,9,152790,19,1]]},"library_calls":["sqlite3_open","sqlite3_drop_modules","sqlite3_exec","sqlite3_errmsg","sqlite3_close_v2"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file", "wb");
    
    // Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Open a SQLite database connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Error opening SQLite database: %s\n", sqlite3_errmsg(db));
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Drop all the modules from the database
    const char *azKeep[1] = { NULL };
    rc = sqlite3_drop_modules(db, azKeep);
    if (rc != SQLITE_OK) {
        // Error handling
        fprintf(out_file, "Error dropping modules: %s\n", sqlite3_errmsg(db));
        sqlite3_close_v2(db);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Execute the statements in the input file
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, in_file)) != -1) {
        rc = sqlite3_exec(db, line, NULL, 0, NULL);
        if (rc != SQLITE_OK) {
            // Error handling
            fprintf(out_file, "Error executing SQL: %s\n", sqlite3_errmsg(db));
            sqlite3_close_v2(db);
            assert_file_closed(&out_file);;
            assert_file_closed(&in_file);;
            if (line) free(line);
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }
    
    // Cleanup
    sqlite3_close_v2(db);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    if (line) free(line);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}