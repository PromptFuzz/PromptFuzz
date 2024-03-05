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
//<ID> 191
//<Prompt> ["sqlite3_create_collation16","sqlite3_db_handle","sqlite3_total_changes","sqlite3_free_filename","sqlite3_progress_handler"]
/*<Combination>: [int sqlite3_create_collation16(sqlite3 * , const void * zName, int eTextRep, void * pArg, int (*)(void *, int, const void *, int, const void *) xCompare),
    sqlite3 *sqlite3_db_handle(sqlite3_stmt * ),
    int sqlite3_total_changes(sqlite3 * ),
    void sqlite3_free_filename(sqlite3_filename ),
    void sqlite3_progress_handler(sqlite3 * , int , int (*)(void *) , void * )
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":6,"density":3,"covered_api":["sqlite3_db_handle","sqlite3_total_changes"],"uncovered_api":["sqlite3_create_collation16","sqlite3_free_filename","sqlite3_progress_handler"],"unique_branches":{"sqlite3_db_handle":[[91073,10,91073,15,1]]},"library_calls":["sqlite3_open","sqlite3_db_handle","sqlite3_total_changes","sqlite3_close"]}
/**/


// Event: Using SQLite3 APIs to create a collation, retrieve the database handle, get total changes, free filename and set progress handler.

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 4: Create input file stream for reading data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file)
    {
        std::cerr << "Failed to open input file stream." << std::endl;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Directly use "input_file" as the file name
    const char *input_file = "input_file";

    sqlite3 *db;
    sqlite3_stmt *stmt;

    // Step 1: Create collation
    int rc = sqlite3_open(input_file, &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Retrieve database handle
    sqlite3 *db_handle = sqlite3_db_handle(stmt);
    
    // Step 3: Get total changes
    int total_changes = sqlite3_total_changes(db_handle);

    // Step 5: Close input file stream and release resources
    assert_file_closed(&in_file);;

    // Step 7: Close database and release resources
    sqlite3_close(db);

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}