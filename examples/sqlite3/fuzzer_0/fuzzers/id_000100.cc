#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2559
//<Prompt> ["sqlite3_test_control","sqlite3_value_text","sqlite3_table_column_metadata","sqlite3_value_blob","sqlite3_bind_double"]
/*<Combination>: [int sqlite3_test_control(int op),
    const unsigned char *sqlite3_value_text(sqlite3_value * ),
    int sqlite3_table_column_metadata(sqlite3 * db, const char * zDbName, const char * zTableName, const char * zColumnName, const char ** pzDataType, const char ** pzCollSeq, int * pNotNull, int * pPrimaryKey, int * pAutoinc),
    const void *sqlite3_value_blob(sqlite3_value * ),
    int sqlite3_bind_double(sqlite3_stmt * , int , double )
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":21,"density":21,"covered_api":["sqlite3_bind_double"],"uncovered_api":["sqlite3_test_control","sqlite3_value_text","sqlite3_table_column_metadata","sqlite3_value_blob"],"unique_branches":{"columnMem":[[90427,29,90427,46,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_text","sqlite3_column_double","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg;

    // Open a temporary database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create a temporary table
    const char *createTableQuery = "CREATE TABLE Test (id INTEGER PRIMARY KEY, name TEXT)";
    if (sqlite3_exec(db, createTableQuery, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 0;
    }

    // Insert data into the table
    const char *insertQuery = "INSERT INTO Test (id, name) VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare input data for binding parameters
    int id = 1;
    const char *name = "John";
    double value = 1.23;

    // Bind parameters to the prepared statement
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, value);

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Query the inserted data
    const char *selectQuery = "SELECT * FROM Test";
    if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Fetch the rows and print the results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int fetchedId = sqlite3_column_int(stmt, 0);
        const unsigned char *fetchedName = sqlite3_column_text(stmt, 1);
        double fetchedValue = sqlite3_column_double(stmt, 2);

        printf("id: %d, name: %s, value: %f\n", fetchedId, fetchedName, fetchedValue);
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Close the database
    sqlite3_close(db);

    return 0;
}