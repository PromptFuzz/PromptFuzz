#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 517
//<Prompt> ["sqlite3_bind_parameter_count","sqlite3_step","sqlite3_column_origin_name16","sqlite3_result_value","sqlite3_free"]
/*<Combination>: [int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_step(sqlite3_stmt * ),
    const void *sqlite3_column_origin_name16(sqlite3_stmt * , int ),
    void sqlite3_result_value(sqlite3_context * , sqlite3_value * ),
    void sqlite3_free(void * )
*/
//<score> 0.6, nr_unique_branch: 0, p_cov: 0.6
//<Quality> {"diversity":13,"density":13,"covered_api":["sqlite3_bind_parameter_count","sqlite3_step","sqlite3_column_origin_name16"],"uncovered_api":["sqlite3_result_value","sqlite3_free"],"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_column_origin_name16","sqlite3_column_value","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 1: Initialize database and prepare statement
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    // Step 2: Open in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Step 3: Create a temporary table
    const char *createTableQuery = "CREATE TABLE temp_table (id INTEGER PRIMARY KEY, name TEXT)";
    rc = sqlite3_exec(db, createTableQuery, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step 4: Prepare INSERT statement
    const char *insertQuery = "INSERT INTO temp_table (id, name) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insertQuery, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Step 5: Bind parameter count
    int paramCount = sqlite3_bind_parameter_count(stmt);
    
    // Step 6: Bind parameters and step
    for (int i = 1; i <= paramCount; i++) {
        // Bind integer parameter
        int parameterValue = 10;
        sqlite3_bind_int(stmt, i, parameterValue);
        
        // Bind text parameter
        const char *textValue = "Test";
        sqlite3_bind_text(stmt, i, textValue, -1, SQLITE_TRANSIENT);
    }
    
    // Step 7: Step the statement
    while (sqlite3_step(stmt) == SQLITE_ROW);
    
    // Step 8: Retrieve column origin name
    const void *columnOriginName = sqlite3_column_origin_name16(stmt, 0);
    
    // Step 9: Retrieve result value
    sqlite3_value *resultValue = sqlite3_column_value(stmt, 0);
    
    // Step 10: Free resources
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}