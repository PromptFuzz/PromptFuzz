#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1575
//<Prompt> ["sqlite3_prepare_v2","sqlite3_filename_journal","sqlite3_bind_parameter_count","sqlite3_aggregate_count","sqlite3_step"]
/*<Combination>: [int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    const char *sqlite3_filename_journal(sqlite3_filename ),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_aggregate_count(sqlite3_context * ),
    int sqlite3_step(sqlite3_stmt * )
*/
//<score> 0.6, nr_unique_branch: 0, p_cov: 0.6
//<Quality> {"diversity":15,"density":15,"covered_api":["sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step"],"uncovered_api":["sqlite3_filename_journal","sqlite3_aggregate_count"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_null","sqlite3_step","sqlite3_column_text","sqlite3_column_bytes","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create input file using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create SQLite database and prepare statement
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *tail;
    int rc;

    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Read SQL query from input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    char *sql = new char[file_size + 1];
    if (!sql) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fread(sql, file_size, 1, in_file);
    sql[file_size] = '\0';
    
    // Prepare SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        delete[] sql;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Bind parameters
    int num_params = sqlite3_bind_parameter_count(stmt);
    for (int i = 1; i <= num_params; i++) {
        if (i%3 == 0) {
            sqlite3_bind_null(stmt, i);
        } else if (i%3 == 1) {
            sqlite3_bind_double(stmt, i, (double)i);
        } else {
            sqlite3_bind_int(stmt, i, i);
        }
    }
    
    // Execute the prepared statement
    while ((rc = sqlite3_step(stmt)) != SQLITE_DONE) {
        if (rc != SQLITE_ROW) {
            delete[] sql;
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }
    
    // Write output data to file
    const unsigned char *result = sqlite3_column_text(stmt, 0);
    size_t result_size = sqlite3_column_bytes(stmt, 0);
    fwrite(result, result_size, 1, out_file);
    
    // Cleanup
    delete[] sql;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}