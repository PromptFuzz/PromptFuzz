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
//<ID> 916
//<Prompt> ["sqlite3_open","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt"]
/*<Combination>: [int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_db_status(sqlite3 * , int op, int * pCur, int * pHiwtr, int resetFlg),
    int sqlite3_keyword_name(int , const char ** , int * ),
    int sqlite3_memory_alarm(void (*)(void *, sqlite3_int64, int) , void * , sqlite3_int64 ),
    sqlite3_filename sqlite3_db_filename(sqlite3 * db, const char * zDbName),
    void sqlite3_interrupt(sqlite3 * )
*/
//<score> 1.4285715, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":7,"density":5,"covered_api":["sqlite3_open","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt"],"uncovered_api":[],"unique_branches":{"sqlite3_keyword_name":[[175411,7,175411,10,0]]},"library_calls":["sqlite3_open","sqlite3_db_status","sqlite3_keyword_name","sqlite3_memory_alarm","sqlite3_db_filename","sqlite3_interrupt","sqlite3_close"]}
/**/

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<=16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Create a FILE pointer to write output data
    FILE *out_file = fopen("output_file", "wb");

    // Create a SQLite database connection pointer
    sqlite3 *db;

    // Open the database connection
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // Handle error if the database connection fails
        fputs("Cannot open database", out_file);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the status of the database
    int cur, hiwtr;
    sqlite3_db_status(db, fuzz_int32_t_1, &cur, &hiwtr, fuzz_int32_t_2);

    // Get the keyword names
    const char *keyword;
    int keywordId;
    sqlite3_keyword_name(-1, &keyword, &keywordId);

    // Set the memory alarm callback
    sqlite3_memory_alarm(nullptr, nullptr, fuzz_int64_t_3);

    // Get the database filename
    sqlite3_filename filename = sqlite3_db_filename(db, NULL);

    // Interrupt the database operation
    sqlite3_interrupt(db);

    // Release resources
    sqlite3_close(db);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}