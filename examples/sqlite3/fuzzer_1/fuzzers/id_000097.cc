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
//<ID> 2533
//<Prompt> ["sqlite3_value_text","sqlite3_result_int","sqlite3_table_column_metadata","sqlite3_create_module_v2","sqlite3_bind_double"]
/*<Combination>: [const unsigned char *sqlite3_value_text(sqlite3_value * ),
    void sqlite3_result_int(sqlite3_context * , int ),
    int sqlite3_table_column_metadata(sqlite3 * db, const char * zDbName, const char * zTableName, const char * zColumnName, const char ** pzDataType, const char ** pzCollSeq, int * pNotNull, int * pPrimaryKey, int * pAutoinc),
    int sqlite3_create_module_v2(sqlite3 * db, const char * zName, const sqlite3_module * p, void * pClientData, void (*)(void *) xDestroy),
    int sqlite3_bind_double(sqlite3_stmt * , int , double )
*/
//<score> 2.4, nr_unique_branch: 11, p_cov: 0.2
//<Quality> {"diversity":10,"density":10,"covered_api":["sqlite3_table_column_metadata"],"uncovered_api":["sqlite3_value_text","sqlite3_result_int","sqlite3_create_module_v2","sqlite3_bind_double"],"unique_branches":{"sqlite3_table_column_metadata":[[180360,7,180360,12,1],[180366,7,180366,21,1],[180369,17,180369,32,0],[180371,11,180371,56,0],[180375,9,180375,25,1],[180396,7,180396,11,0],[180401,15,180401,32,1],[180406,7,180406,16,0],[180423,24,180423,29,1],[180429,32,180429,39,1],[18107,29,18107,57,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_table_column_metadata","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t* f_data, size_t f_size) {
	if(f_size<=19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *errMsg = nullptr;
    
    // Open database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create table
    const char *createTableSQL = "CREATE TABLE test (id INT, name TEXT);";
    rc = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return 0;
    }
    
    // Prepare insert statement
    const char *insertSQL = "INSERT INTO test (id, name) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Bind parameters
    int id = 123;
    const char *name = "John Doe";
    sqlite3_bind_int(stmt, 1, fuzz_int32_t_1);
    sqlite3_bind_text(stmt, fuzz_int32_t_3, fuzz_str_2, static_cast<int>(fuzz_str_sz_2), SQLITE_STATIC);
    
    // Execute insert statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        return 0;
    }
    
    // Finalize statement
    sqlite3_finalize(stmt);
    
    // Query table metadata
    const char *dataType;
    const char *collSeq;
    int notNull;
    int primaryKey;
    int autoinc;
    rc = sqlite3_table_column_metadata(db, nullptr, "test", "id",
                                       &dataType, &collSeq, &notNull,
                                       &primaryKey, &autoinc);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Print metadata
    printf("Data type: %s\n", dataType);
    printf("Collating sequence: %s\n", collSeq);
    printf("Not NULL: %d\n", notNull);
    printf("Primary key: %d\n", primaryKey);
    printf("Auto increment: %d\n", autoinc);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}