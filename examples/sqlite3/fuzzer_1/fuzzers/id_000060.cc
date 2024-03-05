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
//<ID> 1546
//<Prompt> ["sqlite3_filename_journal","sqlite3_data_count","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [const char *sqlite3_filename_journal(sqlite3_filename ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 0.8, nr_unique_branch: 1, p_cov: 0.4
//<Quality> {"diversity":18,"density":18,"covered_api":["sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_filename_journal","sqlite3_data_count","sqlite3_bind_parameter_count"],"unique_branches":{"sqlite3AddPrimaryKey":[[121266,7,121266,14,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<=75) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *filename = "database.db";
    int result = sqlite3_open(filename, &db);
    if (result != SQLITE_OK) {
        return 0;
    }

    char *errmsg;
    result = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, &errmsg);
    if (result != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data from input file and insert into the database
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in_file) != NULL) {
        result = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, NULL);
        if (result != SQLITE_OK) {
            assert_file_closed(&in_file);;
            sqlite3_close(db);
            assert_file_closed(&in_file);
	return 0;
        }
        sqlite3_bind_text(stmt, fuzz_int32_t_2, buffer, -1, SQLITE_STATIC);
        result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
            assert_file_closed(&in_file);;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            assert_file_closed(&in_file);
	return 0;
        }
        sqlite3_finalize(stmt);
    }

    assert_file_closed(&in_file);;

    // Query the number of rows in the table
    result = sqlite3_prepare_v2(db, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), &stmt, NULL);
    if (result != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, fuzz_int32_t_4);
        printf("Number of rows in table: %d\n", count);
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    assert_file_closed(&in_file);
	return 0;
}