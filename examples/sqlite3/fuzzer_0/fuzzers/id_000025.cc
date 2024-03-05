#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 842
//<Prompt> ["sqlite3_enable_shared_cache","sqlite3_vfs_find","sqlite3_interrupt","sqlite3_expired","sqlite3_strnicmp","sqlite3_vtab_rhs_value"]
/*<Combination>: [int sqlite3_enable_shared_cache(int ),
    sqlite3_vfs *sqlite3_vfs_find(const char * zVfsName),
    void sqlite3_interrupt(sqlite3 * ),
    int sqlite3_expired(sqlite3_stmt * ),
    int sqlite3_strnicmp(const char * , const char * , int ),
    int sqlite3_vtab_rhs_value(sqlite3_index_info * , int , sqlite3_value ** ppVal)
*/
//<score> 0.33333334, nr_unique_branch: 1, p_cov: 0.33333334
//<Quality> {"diversity":4,"density":2,"covered_api":["sqlite3_enable_shared_cache","sqlite3_vfs_find"],"uncovered_api":["sqlite3_interrupt","sqlite3_expired","sqlite3_strnicmp","sqlite3_vtab_rhs_value"],"unique_branches":{"openDatabase":[[179744,13,179744,51,0]]},"library_calls":["sqlite3_enable_shared_cache","sqlite3_vfs_find","sqlite3_open","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
  
    // Step 5: Create a file descriptor for reading
    int fd_in = fuzz_fileno(in_file);
  
    // Step 6: Create a string of the input file name
    const char *input_file = "input_file";
  
    // Step 1: Enable shared cache
    sqlite3_enable_shared_cache(1);
    
    // Step 2: Find the vfs by name
    sqlite3_vfs *vfs = sqlite3_vfs_find("vfs_name");
    
    // Step 3: Create a sqlite3 database
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    
    // Step 7: Release resources before returning
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}