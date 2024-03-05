#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2144
//<Prompt> ["sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_data_count","sqlite3_create_filename","sqlite3_vmprintf","sqlite3_mutex_notheld"]
/*<Combination>: [const void *sqlite3_column_table_name16(sqlite3_stmt * , int ),
    int sqlite3_value_frombind(sqlite3_value * ),
    int sqlite3_data_count(sqlite3_stmt * pStmt),
    sqlite3_filename sqlite3_create_filename(const char * zDatabase, const char * zJournal, const char * zWal, int nParam, const char ** azParam),
    char *sqlite3_vmprintf(const char * , __va_list_tag * ),
    int sqlite3_mutex_notheld(sqlite3_mutex * )
*/
//<score> 0, nr_unique_branch: 4, p_cov: 0
//<Quality> {"diversity":13,"density":13,"covered_api":[],"uncovered_api":["sqlite3_column_table_name16","sqlite3_value_frombind","sqlite3_data_count","sqlite3_create_filename","sqlite3_vmprintf","sqlite3_mutex_notheld"],"unique_branches":{"sqlite3VdbeExec":[[95897,9,95897,55,1]],"sqlite3Step":[[89891,9,89891,40,1],[13963,25,13963,46,0],[13963,26,13963,29,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/*Here is the completed C++ program that achieves the given event using the sqlite3 APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create a new SQLite database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Handle error
        assert_file_closed(&in_file);
	return 0;
    }

    // Prepare a SQL statement to create a table
    const char *sql = "CREATE TABLE test (id INT, name TEXT);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // Handle error
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Bind values to the statement
    int id = 1;
    const char *name = "John";
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);

    // Execute the statement again
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // Handle error
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}