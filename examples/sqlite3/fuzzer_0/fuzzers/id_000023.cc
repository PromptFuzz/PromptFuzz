#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 739
//<Prompt> ["sqlite3_column_origin_name","sqlite3_free","sqlite3_blob_read","sqlite3_db_config","sqlite3_value_text"]
/*<Combination>: [const char *sqlite3_column_origin_name(sqlite3_stmt * , int ),
    void sqlite3_free(void * ),
    int sqlite3_blob_read(sqlite3_blob * , void * Z, int N, int iOffset),
    int sqlite3_db_config(sqlite3 * , int op),
    const unsigned char *sqlite3_value_text(sqlite3_value * )
*/
//<score> 0.4, nr_unique_branch: 1, p_cov: 0.2
//<Quality> {"diversity":9,"density":9,"covered_api":["sqlite3_free"],"uncovered_api":["sqlite3_column_origin_name","sqlite3_blob_read","sqlite3_db_config","sqlite3_value_text"],"unique_branches":{"renameTableTest":[[116286,15,116286,21,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_finalize","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open database connection
    sqlite3 *db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read data from input file
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in_file) != NULL) {
        // Execute SQL statements
        char *errMsg = nullptr;
        if (sqlite3_exec(db, buffer, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            sqlite3_free(errMsg);
        }
    }
    
    // Query and print results
    const char *query = "SELECT * FROM mytable";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int numCols = sqlite3_column_count(stmt);
            for (int i = 0; i < numCols; i++) {
                const unsigned char *value = sqlite3_column_text(stmt, i);
                if (value != nullptr) {
                    printf("%s\n", value);
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Close database connection
    sqlite3_close(db);
    
    // Close input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}