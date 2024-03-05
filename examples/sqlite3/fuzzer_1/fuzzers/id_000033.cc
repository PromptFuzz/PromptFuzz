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
//<ID> 959
//<Prompt> ["sqlite3_prepare_v2","sqlite3_sourceid","sqlite3_bind_parameter_count","sqlite3_msize","sqlite3_complete","sqlite3_finalize","sqlite3_interrupt"]
/*<Combination>: [int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    const char *sqlite3_sourceid(),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    sqlite3_uint64 sqlite3_msize(void * ),
    int sqlite3_complete(const char * sql),
    int sqlite3_finalize(sqlite3_stmt * pStmt),
    void sqlite3_interrupt(sqlite3 * )
*/
//<score> 0.6, nr_unique_branch: 0, p_cov: 1
//<Quality> {"diversity":10,"density":6,"covered_api":["sqlite3_prepare_v2","sqlite3_sourceid","sqlite3_bind_parameter_count","sqlite3_msize","sqlite3_complete","sqlite3_finalize","sqlite3_interrupt"],"uncovered_api":[],"unique_branches":{},"library_calls":["sqlite3_initialize","sqlite3_open_v2","sqlite3_prepare_v2","sqlite3_sourceid","sqlite3_bind_parameter_count","sqlite3_msize","sqlite3_complete","sqlite3_finalize","sqlite3_interrupt","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<=44) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Step 4: Create a FILE * variable for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Provide the input file name
    const char *input_file = "input_file";

    // Step 5: Get the file descriptor for reading
    int in_file_descriptor = fuzz_fileno(in_file);

    // Step 4: Create a FILE * variable for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_file_descriptor);
	return 0;
    }

    // Step 6: Provide the output file name
    const char *output_file = "output_file";

    // Step 5: Get the file descriptor for writing
    int out_file_descriptor = fuzz_fileno(out_file);

    // Step 1: Call sqlite3 APIs to achieve the desired event
    // Initialize sqlite3
    sqlite3_initialize();

    // Prepare a SQL statement
    sqlite3 *db;
    const char *sql = "SELECT * FROM table";
    sqlite3_stmt *stmt;
    sqlite3_open_v2(input_file, &db, SQLITE_OPEN_READWRITE, NULL);
    sqlite3_prepare_v2(db, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &stmt, NULL);

    // Get source ID
    const char *source_id = sqlite3_sourceid();

    // Get the number of bind parameters in the statement
    int parameter_count = sqlite3_bind_parameter_count(stmt);

    // Get the size of allocated memory chunk
    void *ptr = malloc(10);
    sqlite3_uint64 memory_size = sqlite3_msize(ptr);

    // Check if the SQL statement is complete
    const char *sql_complete = "SELECT * FROM table";
    int is_complete = sqlite3_complete(fuzz_str_2);

    // Finalize the prepared statement
    sqlite3_finalize(stmt);

    // Interrupt an ongoing operation
    sqlite3_interrupt(db);

    // Step 7: Release allocated resources
    free(ptr);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_file_descriptor);
	assert_fd_closed(in_file_descriptor);
	return 0;
}