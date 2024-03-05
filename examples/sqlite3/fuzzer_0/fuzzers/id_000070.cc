#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1713
//<Prompt> ["sqlite3_open","sqlite3_free","sqlite3_column_int64","sqlite3_complete","sqlite3_is_interrupted"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    void sqlite3_free(void * ),
    sqlite3_int64 sqlite3_column_int64(sqlite3_stmt * , int iCol),
    int sqlite3_complete(const char * sql),
    int sqlite3_is_interrupted(sqlite3 * )
*/
//<score> 10.200001, nr_unique_branch: 16, p_cov: 0.6
//<Quality> {"diversity":5,"density":5,"covered_api":["sqlite3_open","sqlite3_free","sqlite3_complete"],"uncovered_api":["sqlite3_column_int64","sqlite3_is_interrupted"],"unique_branches":{"sqlite3_complete":[[176350,29,176350,67,0],[176352,35,176352,70,0],[176354,35,176354,75,0]],"sqlite3Pragma":[[137725,20,137725,33,1],[137725,37,137725,58,0],[137854,9,137854,16,1],[137904,35,137904,39,0]],"sqlite3VdbeExec":[[100704,11,100704,34,0]],"invalidateTempStorage":[[136661,9,136661,24,0]],"sqlite3VdbeDisplayComment":[[85527,58,85527,68,0]],"changeTempStorage":[[136686,7,136686,51,0]],"sqlite3PcacheFetchStress":[[53858,35,53858,38,1],[53860,9,53860,12,1]],"memTracePrint":[[92387,13,92387,32,0]],"sqlite3VdbeMemPrettyPrint":[[92361,40,92361,47,1],[92361,49,92361,56,1]]},"library_calls":["sqlite3_open","sqlite3_complete","sqlite3_exec","sqlite3_free","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open a temporary database
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Read and Execute SQLite statements from the input file
    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, in_file)) != -1) {
        // Remove trailing newline character
        line[strcspn(line, "\n")] = '\0';

        // Check if the SQLite statement is complete
        if (sqlite3_complete(line)) {
            // Execute the SQLite statement
            char *errmsg;
            rc = sqlite3_exec(db, line, nullptr, nullptr, &errmsg);
            if (rc != SQLITE_OK) {
                // Handle error
                sqlite3_free(errmsg);
            }
        }
    }

    // Cleanup
    free(line);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&in_file);
	return 0;
}