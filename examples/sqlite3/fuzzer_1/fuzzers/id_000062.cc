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
//<ID> 1574
//<Prompt> ["sqlite3_prepare_v2","sqlite3_filename_journal","sqlite3_bind_parameter_count","sqlite3_aggregate_count","sqlite3_step"]
/*<Combination>: [int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    const char *sqlite3_filename_journal(sqlite3_filename ),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_aggregate_count(sqlite3_context * ),
    int sqlite3_step(sqlite3_stmt * )
*/
//<score> 1.2, nr_unique_branch: 2, p_cov: 0.4
//<Quality> {"diversity":5,"density":5,"covered_api":["sqlite3_prepare_v2","sqlite3_step"],"uncovered_api":["sqlite3_filename_journal","sqlite3_bind_parameter_count","sqlite3_aggregate_count"],"unique_branches":{"sqlite3PagerOpen":[[61174,11,61174,20,1]],"sqlite3BtreeOpen":[[72214,9,72214,44,0]]},"library_calls":["sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size)
{
	if(f_size<=22) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Step 4: Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file)
    {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Set input file name
    const char *input_file = "input_file";

    sqlite3 *db;
    sqlite3_stmt *stmt;

    // Step 7: Initialize db
    if (sqlite3_open_v2(input_file, &db, SQLITE_OPEN_READONLY, nullptr) != SQLITE_OK)
    {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 7: Prepare SQL statement
    const char *sql = "SELECT * FROM table";
    sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, nullptr);

    // Step 7: Execute SQL statement
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        // Process row data
    }

    // Step 7: Finalize SQL statement
    sqlite3_finalize(stmt);

    // Step 7: Close database
    sqlite3_close(db);

    // Step 4: Close input file
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}