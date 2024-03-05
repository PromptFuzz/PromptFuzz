#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 31
//<Prompt> ["deflateParams","inflateSyncPoint","inflateResetKeep","zlibVersion","gzfread","deflateSetHeader","inflateValidate","inflateBackInit_","zlibCompileFlags"]
/*<Combination>: [int deflateParams(z_streamp strm, int level, int strategy),
    int inflateSyncPoint(z_streamp ),
    int inflateResetKeep(z_streamp ),
    const char *zlibVersion(),
    z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    int inflateValidate(z_streamp , int ),
    int inflateBackInit_(z_streamp strm, int windowBits, unsigned char * window, const char * version, int stream_size),
    uLong zlibCompileFlags()
*/
//<score> 10.5, nr_unique_branch: 11
//<Quality> {"density":3,"unique_branches":{"inflateBackInit_":[[30,9,30,26,0,0,4,1],[30,30,30,59,0,0,4,1],[31,9,31,47,0,0,4,1],[33,9,33,23,0,0,4,1],[33,27,33,43,0,0,4,0]],"gzfread":[[382,9,382,21,0,0,4,1],[387,9,387,31,0,0,4,0]],"inflateValidate":[[1501,9,1501,32,0,0,4,0]],"deflateParams":[[760,9,760,32,0,0,4,0]],"deflateSetHeader":[[710,9,710,32,0,0,4,0]],"inflateSyncPoint":[[1434,9,1434,32,0,0,4,0]]},"library_calls":["gzdopen","deflateParams","inflateSyncPoint","inflateResetKeep","zlibVersion","gzfread","deflateSetHeader","inflateValidate","inflateBackInit_","zlibCompileFlags","gzclose"],"critical_calls":["gzdopen","deflateParams","inflateSyncPoint","inflateResetKeep","zlibVersion","gzfread","deflateSetHeader","inflateValidate","inflateBackInit_","zlibCompileFlags","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
  
    // Step 6: Create a file name string
    const char *output_file = "output_file";

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(fopen(output_file, "wb"));
    if (out_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }

    // Step 2: Use zlib APIs to achieve the event
    z_streamp strm = new z_stream();
    gzFile file = gzdopen(dup(out_fd), "wb");

    // Call zlib APIs
    int status = deflateParams(strm, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    status |= inflateSyncPoint(strm);
    status |= inflateResetKeep(strm);

    // Step 3: Consume the input data
    char buffer[size];
    fread(buffer, 1, size, in_file);

    // Call zlib APIs
    const char *version = zlibVersion();
    z_size_t read_size = gzfread(buffer, 1, size, file);
    gz_header header;
    status |= deflateSetHeader(strm, &header);
    status |= inflateValidate(strm, Z_DEFAULT_COMPRESSION);
    status |= inflateBackInit_(strm, MAX_WBITS, NULL, version, sizeof(z_stream));
    uLong compile_flags = zlibCompileFlags();

    // Clean up
    assert_file_closed(&in_file);;
    close(out_fd);
    gzclose(file);
    delete strm;

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}