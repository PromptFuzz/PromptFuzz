#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2347
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation16","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    int sqlite3_create_collation16(sqlite3 * , const void * zName, int eTextRep, void * pArg, int (*)(void *, int, const void *, int, const void *) xCompare),
    int sqlite3_get_autocommit(sqlite3 * ),
    int sqlite3_config(int ),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 0.85714287, nr_unique_branch: 1, p_cov: 0.85714287
//<Quality> {"diversity":12,"density":6,"covered_api":["sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf"],"uncovered_api":["sqlite3_create_collation16"],"unique_branches":{"sqlite3_create_collation_v2":[[180150,36,180150,44,0]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_last_insert_rowid","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_create_collation_v2","sqlite3_get_autocommit","sqlite3_config","sqlite3_snprintf","sqlite3_close","sqlite3_shutdown"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a file to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a file to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptor for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize the SQLite library
    sqlite3_initialize();

    // Open a database
    sqlite3 *db;
    int result = sqlite3_open(":memory:", &db);
    if (result != SQLITE_OK) {
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

    // Execute SQLite API calls
    sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);
    int blob_size = 100;
    const void *blob_data = "Hello, World";
    result = sqlite3_bind_blob(nullptr, 0, blob_data, blob_size, nullptr);
    int keyword_count = sqlite3_keyword_count();
    sqlite3_create_collation_v2(db, nullptr, 0, nullptr, nullptr, nullptr);
    int autocommit = sqlite3_get_autocommit(db);
    sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
    char buf[100];
    sqlite3_snprintf(100, buf, "Value is %d", 2020);

    // Write the output data to the file
    fwrite(&last_row_id, sizeof(sqlite3_int64), 1, out_file);
    fwrite(&blob_size, sizeof(int), 1, out_file);
    fwrite(&keyword_count, sizeof(int), 1, out_file);
    fwrite(&autocommit, sizeof(int), 1, out_file);
    fwrite(&result, sizeof(int), 1, out_file);
    fwrite(buf, sizeof(char), strlen(buf), out_file);

    // Close the database
    sqlite3_close(db);

    // Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Cleanup
    sqlite3_shutdown();

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}