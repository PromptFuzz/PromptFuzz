#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 519
//<Prompt> ["gzopen64","inflateReset","gzopen","deflateInit_","inflateCopy","gzwrite","deflateEnd","gzbuffer","deflateParams","gzclose"]
/*<Combination>: [gzFile gzopen64(const char * , const char * ),
    int inflateReset(z_streamp strm),
    gzFile gzopen(const char * , const char * ),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int inflateCopy(z_streamp dest, z_streamp source),
    int gzwrite(gzFile file, voidpc buf, unsigned int len),
    int deflateEnd(z_streamp strm),
    int gzbuffer(gzFile file, unsigned int size),
    int deflateParams(z_streamp strm, int level, int strategy),
    int gzclose(gzFile file)
*/
//<score> 12, nr_unique_branch: 5
//<Quality> {"density":6,"unique_branches":{"gz_write":[[192,9,192,26,0,0,4,1],[215,13,215,33,0,0,4,1],[222,17,222,24,0,0,4,0],[226,17,226,49,0,0,4,1],[229,18,229,21,0,0,4,1]]},"library_calls":["gzopen64","inflateReset","gzopen","deflateInit_","zlibVersion","inflateCopy","gzwrite","deflateEnd","gzbuffer","deflateParams","gzclose","gzclose"],"critical_calls":["gzopen64","inflateReset","gzopen","deflateInit_","zlibVersion","inflateCopy","gzwrite","deflateEnd","gzbuffer","deflateParams","gzclose","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Creating the input and output file variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Getting file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Achieving the event using zlib APIs
    gzFile input_gzfile = gzopen64("input_file", "rb");
    z_stream input_strm;
    inflateReset(&input_strm);
    gzFile output_gzfile = gzopen("output_file", "wb");
    z_stream output_strm;
    deflateInit_(&output_strm, Z_DEFAULT_COMPRESSION, zlibVersion(), sizeof(z_stream));
    inflateCopy(&input_strm, &output_strm);
    gzwrite(output_gzfile, (voidpc)data, size);
    deflateEnd(&output_strm);
    gzbuffer(output_gzfile, 1024);
    deflateParams(&output_strm, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    gzclose(output_gzfile);

    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    gzclose(input_gzfile);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}