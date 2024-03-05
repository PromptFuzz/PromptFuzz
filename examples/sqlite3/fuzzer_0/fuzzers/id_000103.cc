#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2648
//<Prompt> ["sqlite3_mutex_alloc","sqlite3_status","sqlite3_trace","sqlite3_column_count","sqlite3_compileoption_get","sqlite3_keyword_count"]
/*<Combination>: [sqlite3_mutex *sqlite3_mutex_alloc(int ),
    int sqlite3_status(int op, int * pCurrent, int * pHighwater, int resetFlag),
    void *sqlite3_trace(sqlite3 * , void (*)(void *, const char *) xTrace, void * ),
    int sqlite3_column_count(sqlite3_stmt * pStmt),
    const char *sqlite3_compileoption_get(int N),
    int sqlite3_keyword_count()
*/
//<score> 0.33333334, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":9,"density":3,"covered_api":["sqlite3_mutex_alloc","sqlite3_status","sqlite3_trace","sqlite3_column_count","sqlite3_compileoption_get","sqlite3_keyword_count"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_mutex_alloc","sqlite3_status","sqlite3_trace","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_compileoption_get","sqlite3_keyword_count","sqlite3_mutex_free","sqlite3_finalize"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    sqlite3_mutex *mutex;
    int status;
    void *trace;
    sqlite3_stmt *stmt;
    const char *compileOption;
    int keywordCount;

    // Open the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Allocate a mutex
    mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);

    // Get the status of sqlite3
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &status, NULL, 0);
    
    // Set a trace function
    trace = sqlite3_trace(NULL, [](void *data, const char *sql) {
        // Do something with the traced SQL statement
    }, NULL);

    // Prepare a statement
    char sql[] = "SELECT * FROM table";
    sqlite3_prepare_v2(NULL, sql, -1, &stmt, NULL);

    // Get the number of columns in the result set
    int columnCount = sqlite3_column_count(stmt);

    // Get a compile option
    compileOption = sqlite3_compileoption_get(0);

    // Get the number of keywords in SQLite
    keywordCount = sqlite3_keyword_count();

    // Close the input file
    assert_file_closed(&in_file);;

    // Release allocated resources
    sqlite3_mutex_free(mutex);
    sqlite3_finalize(stmt);

    assert_file_closed(&in_file);
	return 0;
}