#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1506
//<Prompt> ["deflateReset","deflateInit2_","inflateResetKeep","deflate","gzsetparams","gzgetc","compress2","deflateBound","deflatePrime"]
/*<Combination>: [int deflateReset(z_streamp strm),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    int inflateResetKeep(z_streamp ),
    int deflate(z_streamp strm, int flush),
    int gzsetparams(gzFile file, int level, int strategy),
    int gzgetc(gzFile file),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    uLong deflateBound(z_streamp strm, uLong sourceLen),
    int deflatePrime(z_streamp strm, int bits, int value)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["deflateInit2_","gzdopen","deflateEnd","gzsetparams","deflate","gzwrite","gzflush","gzclose","deflateEnd"],"critical_calls":["deflateInit2_","gzdopen","gzsetparams","deflate","gzwrite","gzflush","gzclose","deflateEnd"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Create a FILE variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize zlib stream for compression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set gzFile parameters
    gzFile gz = gzdopen(out_fd, "wb");
    if (gz == NULL) {
        deflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    gzsetparams(gz, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Read data from input file and compress
    const int chunkSize = 1024;
    uint8_t buf[chunkSize];
    int bytesRead;
    do {
        bytesRead = fread(buf, 1, chunkSize, in_file);
        if (bytesRead > 0) {
            strm.avail_in = bytesRead;
            strm.next_in = buf;

            do {
                strm.avail_out = chunkSize;
                strm.next_out = buf;

                // Compress the data
                deflate(&strm, Z_FINISH);
                int bytesWritten = chunkSize - strm.avail_out;

                // Write the compressed data to output file
                gzwrite(gz, buf, bytesWritten);
            } while (strm.avail_out == 0);
        }
    } while (bytesRead > 0);

    // Flush and close the compressed file
    gzflush(gz, Z_FINISH);
    gzclose(gz);

    // Clean up
    deflateEnd(&strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}