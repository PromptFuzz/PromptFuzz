#include "FDSan.h"
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1071
//<Prompt> ["sqlite3_bind_blob64","sqlite3_msize","sqlite3_prepare_v2","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_error_offset"]
/*<Combination>: [int sqlite3_bind_blob64(sqlite3_stmt * , int , const void * , sqlite3_uint64 , void (*)(void *) ),
    sqlite3_uint64 sqlite3_msize(void * ),
    int sqlite3_prepare_v2(sqlite3 * db, const char * zSql, int nByte, sqlite3_stmt ** ppStmt, const char ** pzTail),
    int sqlite3_backup_pagecount(sqlite3_backup * p),
    int sqlite3_bind_text16(sqlite3_stmt * , int , const void * , int , void (*)(void *) ),
    int sqlite3_error_offset(sqlite3 * db)
*/
//<score> 1.2727273, nr_unique_branch: 1, p_cov: 1
//<Quality> {"diversity":11,"density":7,"covered_api":["sqlite3_bind_blob64","sqlite3_msize","sqlite3_prepare_v2","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_error_offset"],"uncovered_api":[],"unique_branches":{"sqlite3_backup_pagecount":[[81597,7,81597,11,0]]},"library_calls":["sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_backup_pagecount","sqlite3_bind_text16","sqlite3_bind_blob64","sqlite3_msize","sqlite3_error_offset","sqlite3_close"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    sqlite3 *db;
    sqlite3_stmt *stmt;
    sqlite3_backup *backup;

    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    char *sql = nullptr;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&sql, &len, in_file)) != -1) {
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            free(sql);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Consume the data
        }

        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    sqlite3_backup_pagecount(backup);
    sqlite3_bind_text16(stmt, 0, nullptr, 0, SQLITE_STATIC);
    sqlite3_bind_blob64(stmt, 0, nullptr, 0, SQLITE_STATIC);
    sqlite3_msize(nullptr);
    sqlite3_error_offset(db);

    free(sql);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}