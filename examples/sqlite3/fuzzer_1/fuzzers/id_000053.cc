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
//<ID> 1376
//<Prompt> ["sqlite3_libversion_number","sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_extended_result_codes","sqlite3_step","sqlite3_finalize"]
/*<Combination>: [int sqlite3_libversion_number(),
    int sqlite3_open(const char * filename, sqlite3 ** ppDb),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_bind_parameter_count(sqlite3_stmt * ),
    int sqlite3_bind_zeroblob64(sqlite3_stmt * , int , sqlite3_uint64 ),
    int sqlite3_extended_result_codes(sqlite3 * , int onoff),
    int sqlite3_step(sqlite3_stmt * ),
    int sqlite3_finalize(sqlite3_stmt * pStmt)
*/
//<score> 87.5, nr_unique_branch: 99, p_cov: 0.875
//<Quality> {"diversity":8,"density":8,"covered_api":["sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_extended_result_codes","sqlite3_step","sqlite3_finalize"],"uncovered_api":["sqlite3_libversion_number"],"unique_branches":{"sqlite3PagerSetFlags":[[59852,25,59852,55,0]],"getAndInitPage":[[71919,9,71919,22,0]],"moveToLeftmost":[[75092,10,75092,23,1]],"fkScanChildren":[[130059,7,130059,25,0],[130059,29,130059,36,0],[130059,29,130059,36,1],[18143,25,18143,63,0]],"sqlite3InitOne":[[139742,11,139742,22,0],[139756,11,139756,49,0],[139787,7,139787,55,0]],"sqlite3PagerCommitPhaseOne":[[62861,11,62861,18,1]],"moveToChild":[[74916,7,74916,20,1],[74922,7,74922,9,0]],"zeroJournalHdr":[[57683,26,57683,41,1]],"sqlite3VdbeExec":[[93178,9,93178,32,1]],"unixOpen":[[43835,9,43835,41,0],[43948,34,43948,45,1]],"sqlite3FkCheck":[[130471,9,130471,27,1]],"unixTruncate":[[41533,33,41533,41,0],[41543,9,41543,30,0]],"lockTable":[[20715,35,20715,49,0]],"moveToRoot":[[75033,9,75033,22,0]],"sqlite3_backup_step":[[81405,13,81405,29,0],[81408,15,81408,52,1],[81416,13,81416,29,0],[81447,34,81447,47,0],[81447,51,81447,70,0],[81447,51,81447,70,1],[81448,17,81448,39,0],[81451,19,81451,32,0],[81457,15,81457,28,0],[81465,13,81465,26,0],[81465,30,81465,39,1],[81477,15,81477,28,0],[81482,15,81482,28,0],[14484,20,14484,27,1]],"exprTableColumn":[[129955,7,129955,12,0]],"unixDelete":[[44103,7,44103,23,0],[44106,9,44106,22,0],[44107,11,44107,29,1]],"backupOnePage":[[14484,20,14484,27,0]],"writeSuperJournal":[[57972,7,57972,14,1],[57973,7,57973,52,1],[57974,7,57974,14,1],[57982,17,57982,31,0],[57982,17,57982,31,1],[57990,7,57990,23,0],[57998,7,57998,77,1],[57999,7,57999,75,1],[58000,7,58000,71,1],[58001,7,58001,72,1],[58002,7,58003,55,1],[58019,7,58019,66,0],[58020,7,58020,34,1]],"pager_end_transaction":[[58329,30,58329,46,1]],"hasHotJournal":[[61386,13,61386,21,0],[61386,25,61386,34,0],[61388,15,61388,60,0],[61390,17,61390,39,0],[61404,15,61404,28,1],[61414,21,61414,40,0]],"backupTruncateFile":[[81248,7,81248,20,0],[81248,24,81248,38,0]],"pagerOpenSavepoint":[[63134,32,63134,52,1]],"resolveSelectStep":[[107114,11,107114,64,0]],"vdbeCommit":[[86707,7,86707,16,1],[86750,9,86750,18,1],[86754,11,86754,21,1],[86772,13,86772,26,0],[86772,30,86772,33,1],[86773,9,86773,22,0],[86780,9,86780,22,1],[86791,14,86791,23,0],[86791,14,86791,23,1],[86793,11,86793,54,0],[86793,11,86793,54,1],[86795,13,86795,21,1],[86801,13,86801,26,1],[86813,9,86813,80,0],[86814,9,86814,72,1],[86832,14,86832,27,0],[86832,31,86832,40,0],[86832,31,86832,40,1],[86834,11,86834,14,0],[86834,11,86834,14,1],[86840,9,86840,22,1],[86852,9,86852,11,1],[86865,14,86865,23,0],[86865,14,86865,23,1],[86867,11,86867,14,0],[86867,11,86867,14,1]],"sqlite3PagerSharedLock":[[61517,11,61517,24,0],[57066,22,57066,38,0]],"pager_playback":[[59117,9,59117,70,1]],"btreeInitPage":[[71750,7,71750,34,0]],"sqlite3PagerSetPagesize":[[57083,21,57083,41,0]],"decodeFlags":[[71589,15,71589,52,1]],"reindexTable":[[124851,7,124851,17,1]],"sqlite3_extended_result_codes":[[180457,7,180457,32,0]]},"library_calls":["sqlite3_extended_result_codes","sqlite3_open","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_finalize","sqlite3_close"]}
/*Below is the C++ program that utilizes the sqlite3 library APIs to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<=12) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(f_data, sizeof(uint8_t), f_size, input_file_ptr);
	fclose(input_file_ptr);

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    sqlite3_extended_result_codes(db, fuzz_int32_t_1);
    
    char *filename = "input_file";
    sqlite3_open(filename, &db);
    
    // Read the SQLite statements from the input file and execute them
    char line[256];
    while (fgets(line, sizeof(line), in_file) != NULL) {
        int rc = sqlite3_prepare_v2(db, line, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            int param_count = sqlite3_bind_parameter_count(stmt);
            for (int i = 0; i < param_count; i++) {
                sqlite3_bind_zeroblob64(stmt, i+1, fuzz_uint64_t_2);
            }
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Process the rows returned by the statement
                // ...
            }
            sqlite3_finalize(stmt);
        }
    }

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    sqlite3_close(db);
    
    assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}