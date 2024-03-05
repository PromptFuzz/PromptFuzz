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
//<ID> 2214
//<Prompt> ["sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_data_count","sqlite3_vmprintf","sqlite3_mutex_notheld"]
/*<Combination>: [const void *sqlite3_column_table_name16(sqlite3_stmt * , int ),
    int sqlite3_value_frombind(sqlite3_value * ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    int sqlite3_mutex_notheld(sqlite3_mutex * )
*/
//<score> 0.3157895, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":19,"density":15,"covered_api":["sqlite3_column_table_name16"],"uncovered_api":["sqlite3_value_frombind","sqlite3_data_count","sqlite3_vmprintf","sqlite3_mutex_notheld"],"unique_branches":{"columnName":[[90594,7,90594,10,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_column_count","sqlite3_column_table_name16","sqlite3_finalize","sqlite3_close"]}
/*Here is an example of a C++ program that uses the sqlite3 library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<=63) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Step 1: Create a database and open a connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);

    if (rc != SQLITE_OK) {
        std::cout << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 0;
    }

    // Step 2: Create a table
    const char *sql = "CREATE TABLE test (id INT, value TEXT);";
    char *error_msg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &error_msg);

    if (rc != SQLITE_OK) {
        std::cout << "SQL error: " << error_msg << std::endl;
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return 0;
    }

    // Step 3: Prepare a SQL statement
    sql = "INSERT INTO test (id, value) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cout << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 0;
    }

    // Step 4: Bind values to the SQL statement
    int id = 1;
    const char *value = "Test value";
    sqlite3_bind_int(stmt, 1, fuzz_int32_t_2);
    sqlite3_bind_text(stmt, fuzz_int32_t_4, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), SQLITE_STATIC);

    // Step 5: Execute the SQL statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cout << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Step 6: Retrieve the table name from column
    int columnId = sqlite3_column_count(stmt) - 1;
    const void *tableName = sqlite3_column_table_name16(stmt, columnId);

    if (tableName != nullptr) {
        std::cout << "Table name: " << static_cast<const char16_t *>(tableName) << std::endl;
    }

    // Step 7: Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}