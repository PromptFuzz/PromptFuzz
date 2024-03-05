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
//<ID> 2294
//<Prompt> ["sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_bind_blob","sqlite3_keyword_count","sqlite3_snprintf"]
/*<Combination>: [sqlite3_int64 sqlite3_last_insert_rowid(sqlite3 * ),
    int sqlite3_create_function_v2(sqlite3 * db, const char * zFunctionName, int nArg, int eTextRep, void * pApp, void (*)(sqlite3_context *, int, sqlite3_value **) xFunc, void (*)(sqlite3_context *, int, sqlite3_value **) xStep, void (*)(sqlite3_context *) xFinal, void (*)(void *) xDestroy),
    int sqlite3_bind_blob(sqlite3_stmt * , int , const void * , int n, void (*)(void *) ),
    int sqlite3_keyword_count(),
    char *sqlite3_snprintf(int , char * , const char * )
*/
//<score> 0.4909091, nr_unique_branch: 0, p_cov: 0.6
//<Quality> {"diversity":22,"density":18,"covered_api":["sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_bind_blob"],"uncovered_api":["sqlite3_keyword_count","sqlite3_snprintf"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_last_insert_rowid","sqlite3_create_function_v2","sqlite3_result_text","sqlite3_value_int64","sqlite3_result_text","sqlite3_value_int64","sqlite3_exec","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<=91) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
	//fuzzer shim end}




    // Step 1: Open a database in memory
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Step 2: Create a table
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB);";
    rc = sqlite3_exec(db, sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 3: Prepare an INSERT statement
    sqlite3_stmt *stmt;
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 4: Bind the input data as a blob parameter
    rc = sqlite3_bind_blob(stmt, fuzz_int32_t_2, data, size, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 5: Execute the INSERT statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 6: Get the last inserted row id
    sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);

    // Step 7: Create a function to print the row id
    sqlite3_create_function_v2(db, fuzz_str_3, fuzz_int32_t_4, fuzz_int32_t_5, nullptr, [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        sqlite3_int64 row_id = sqlite3_value_int64(argv[0]);
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Row ID: %lld", row_id);
        sqlite3_result_text(ctx, buffer, -1, SQLITE_TRANSIENT);
    }, nullptr, nullptr, nullptr);

    // Step 8: Execute the function to print the row id
    const char *select_sql = "SELECT print_row_id(?);";
    rc = sqlite3_exec(db, fuzz_str_6, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}