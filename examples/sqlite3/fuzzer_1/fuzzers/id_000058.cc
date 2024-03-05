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
//<ID> 1477
//<Prompt> ["sqlite3_data_count","sqlite3_auto_extension","sqlite3_bind_parameter_count","sqlite3_vtab_on_conflict","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [int sqlite3_data_count(sqlite3_stmt * pStmt),
    int sqlite3_auto_extension(void (*)() xEntryPoint),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_vtab_on_conflict(sqlite3 * ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 0.75, nr_unique_branch: 2, p_cov: 0.33333334
//<Quality> {"diversity":12,"density":9,"covered_api":["sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_data_count","sqlite3_auto_extension","sqlite3_bind_parameter_count","sqlite3_vtab_on_conflict"],"unique_branches":{"sqlite3ErrStr":[[178160,5,178160,20,0],[178168,5,178168,12,1]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_errmsg","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<=0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result;
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Open the database
    result = sqlite3_open(":memory:", &db);
    if (result != SQLITE_OK) {
        fprintf(out_file, "Error opening database: %s\n", sqlite3_errmsg(db));
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Execute the SQL statements from the input file
    char *sql = nullptr;
    size_t sql_length = 0;
    ssize_t read;
    while ((read = getline(&sql, &sql_length, in_file)) != -1) {
        // Remove trailing newline character if present
        if (sql[read - 1] == '\n') {
            sql[read - 1] = '\0';
        }
        
        // Prepare the statement
        result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            fprintf(out_file, "Error preparing statement: %s\n", sqlite3_errmsg(db));
            free(sql);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            sqlite3_close(db);
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Execute the statement
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            fprintf(out_file, "Error executing statement: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            free(sql);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            sqlite3_close(db);
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Finalize the statement for reuse
        sqlite3_finalize(stmt);
    }

    free(sql);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}