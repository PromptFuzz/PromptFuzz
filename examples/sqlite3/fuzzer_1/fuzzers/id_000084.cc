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
//<ID> 2227
//<Prompt> ["sqlite3_changes64","sqlite3_column_table_name16","sqlite3_data_count","sqlite3_vmprintf","sqlite3_mutex_notheld"]
/*<Combination>: [sqlite3_int64 sqlite3_changes64(sqlite3 * ),
    const void *sqlite3_column_table_name16(sqlite3_stmt * , int ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    int sqlite3_mutex_notheld(sqlite3_mutex * )
*/
//<score> 1.2, nr_unique_branch: 1, p_cov: 0.6
//<Quality> {"diversity":15,"density":15,"covered_api":["sqlite3_changes64","sqlite3_column_table_name16","sqlite3_data_count"],"uncovered_api":["sqlite3_vmprintf","sqlite3_mutex_notheld"],"unique_branches":{"sqlite3_data_count":[[90372,17,90372,35,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_changes64","sqlite3_column_table_name16","sqlite3_data_count","sqlite3_finalize","sqlite3_close"]}
/*Here is the C++ program that achieves the event using the sqlite3 APIs and follows the provided instructions:

*/


extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* f_data, size_t f_size) {
	if(f_size<=60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    // Create a file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Open a database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a table in the database
    const char *sql = "CREATE TABLE test (id INT, name TEXT);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Prepare a SQL statement
    sql = "INSERT INTO test (id, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Bind values to the prepared statement
    int id = 1;
    const char *name = "John";
    sqlite3_bind_int(stmt, 1, fuzz_int32_t_2);
    sqlite3_bind_text(stmt, fuzz_int32_t_4, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), SQLITE_STATIC);

    // Execute the prepared statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of rows affected by the last INSERT, UPDATE, or DELETE statement
    sqlite3_int64 changes = sqlite3_changes64(db);
    printf("Number of rows affected: %lld\n", changes);

    // Get the table name of a column in a result set
    const void *tableName = sqlite3_column_table_name16(stmt, fuzz_int32_t_5);
    if (tableName != NULL) {
        printf("Table name: %ls\n", (const wchar_t *)tableName);
    }

    // Get the number of columns in a result set
    int columnCount = sqlite3_data_count(stmt);
    printf("Number of columns: %d\n", columnCount);

    // Print the input data from the file
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in_file) != NULL) {
        printf("%s", buffer);
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}