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
//<ID> 119
//<Prompt> ["sqlite3_column_origin_name16","sqlite3_prepare_v2","sqlite3_sourceid","sqlite3_bind_parameter_count","sqlite3_msize"]
/*<Combination>: [const void *sqlite3_column_origin_name16(sqlite3_stmt * , int ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    const char *sqlite3_sourceid(),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    sqlite3_uint64 sqlite3_msize(void * )
*/
//<score> 1.4222223, nr_unique_branch: 1, p_cov: 0.8
//<Quality> {"diversity":9,"density":8,"covered_api":["sqlite3_column_origin_name16","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_msize"],"uncovered_api":["sqlite3_sourceid"],"unique_branches":{"columnName":[[90618,9,90618,17,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_step","sqlite3_column_origin_name16","sqlite3_msize","sqlite3_finalize","sqlite3_close"]}
/*Here is the C++ program that achieves the event using the sqlite3 APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<=4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Open the output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }

    // Create a new database connection
    sqlite3 *db;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    const char *tail;
    rc = sqlite3_prepare_v2(db, (const char*)data, size, &stmt, &tail);
    if (rc != SQLITE_OK) {
        assert_file_closed(&out_file);;
        sqlite3_close(db);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
    }

    // Get the number of bound parameters
    int paramCount = sqlite3_bind_parameter_count(stmt);

    // Write the parameter count to the output file
    fwrite(&paramCount, sizeof(int), 1, out_file);

    // Iterate over the result columns
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get the origin column name
        const void *columnName = sqlite3_column_origin_name16(stmt, fuzz_int32_t_1);

        // Write the column name to the output file
        fwrite(columnName, sizeof(char), sqlite3_msize(const_cast<void*>(columnName)), out_file);
    }

    // Close the statement, close the database, and close the output file
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}