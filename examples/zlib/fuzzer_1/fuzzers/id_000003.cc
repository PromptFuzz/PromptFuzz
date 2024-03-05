#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 33
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
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":4,"unique_branches":{"deflateSetHeader":[[710,9,710,32,0,0,4,1],[710,36,710,58,0,0,4,0]],"deflate":[[993,38,993,57,0,0,4,0]]},"library_calls":["deflateInit_","deflateSetHeader","deflate","deflateEnd"],"critical_calls":["deflateInit_","deflateSetHeader","deflate","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 4. Open input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // 6. Specify output file name
    const char* out_file_name = "output_file";

    // 7. Open output file
    FILE* out_file = fopen(out_file_name, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 8. Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);

    // Create zlib input and output streams
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // 9. Initialize zlib for compression
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set input and output buffers
    unsigned char in_buf[16384];
    unsigned char out_buf[16384];

    // Set zlib version in gz_header
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.done = 1;
    deflateSetHeader(&strm, &header);

    // 10. Read from input file, compress, and write to output file
    while (true) {
        size_t bytes_read = fread(in_buf, 1, sizeof(in_buf), in_file);
        if (bytes_read == 0) {
            break;
        }
        strm.next_in = in_buf;
        strm.avail_in = bytes_read;

        do {
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            int ret = deflate(&strm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                break;
            }
            size_t bytes_written = sizeof(out_buf) - strm.avail_out;
            fwrite(out_buf, 1, bytes_written, out_file);
        } while (strm.avail_out == 0);
    }

    // Release zlib resources
    deflateEnd(&strm);

    // Release file resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}