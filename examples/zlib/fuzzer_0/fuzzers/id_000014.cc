#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 251
//<Prompt> ["gzopen64","gzseek64","inflateReset2","deflateBound","inflateEnd","gztell","deflateInit_","zError"]
/*<Combination>: [gzFile gzopen64(const char * , const char * ),
    off64_t gzseek64(gzFile , off64_t , int ),
    int inflateReset2(z_streamp strm, int windowBits),
    uLong deflateBound(z_streamp strm, uLong sourceLen),
    int inflateEnd(z_streamp strm),
    off_t gztell(gzFile ),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    const char *zError(int )
*/
//<score> 6.6666665, nr_unique_branch: 6
//<Quality> {"density":10,"unique_branches":{"gzseek64":[[352,9,352,27,0,0,4,0],[352,31,352,56,0,0,4,1],[367,35,367,53,0,0,4,0],[368,13,368,39,0,0,4,0],[370,13,370,22,0,0,4,1],[395,38,395,71,0,0,4,0]]},"library_calls":["gzopen64","gzread","gzclose","gzclose","gztell","gzseek64","inflateReset2","deflateInit_","gzclose","deflate","deflateEnd","gzclose","deflate","deflateEnd","gzclose","deflateEnd","gzclose"],"critical_calls":["gzopen64","gzread","gztell","gzseek64","inflateReset2","deflateInit_","deflate","deflate","deflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Define input file name
    const char *input_file = "input_file";

    // Step 1: Open gzipped file for reading
    gzFile gzfile = gzopen64(input_file, "rb");
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 7: Read gzipped file and write to output file
    while (1) {
        char buffer[1024];
        int bytesRead = gzread(gzfile, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            gzclose(gzfile);
            assert_file_closed(&in_file);;
            assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
        }
        if (bytesRead == 0) {
            break;
        }
        // Step 4: Create output file stream
        FILE *out_file = fopen("output_file", "wb");
        if (!out_file) {
            gzclose(gzfile);
            assert_file_closed(&in_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
        // Step 6: Define output file name
        const char *output_file = "output_file";

        // Step 5: Obtain file descriptor for output file
        int out_fd = fuzz_fileno(out_file);

        // Step 2: Seek to a specific position in input file
        off_t pos = gztell(gzfile);
        gzseek64(gzfile, pos, SEEK_SET);

        // Step 3: Reset inflate stream
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        inflateReset2(&strm, -MAX_WBITS);

        // Step 1: Initialize deflate stream for output file
        int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) {
            gzclose(gzfile);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
        }

        // Step 7: Write to output file using deflate
        while (1) {
            strm.avail_out = sizeof(buffer);
            strm.next_out = (Bytef *)buffer;

            ret = deflate(&strm, Z_FINISH);
            if (ret == Z_STREAM_END) {
                break;
            }
            if (ret != Z_OK) {
                deflateEnd(&strm);
                gzclose(gzfile);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
            }
            fwrite(buffer, sizeof(buffer) - strm.avail_out, 1, out_file);
        }

        // Step 7: Write remaining data to output file
        while (1) {
            strm.avail_out = sizeof(buffer);
            strm.next_out = (Bytef *)buffer;

            ret = deflate(&strm, Z_FINISH);
            if (ret == Z_STREAM_END) {
                break;
            }
            if (ret != Z_OK) {
                deflateEnd(&strm);
                gzclose(gzfile);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
            }
            fwrite(buffer, sizeof(buffer) - strm.avail_out, 1, out_file);
        }

        deflateEnd(&strm);
        assert_file_closed(&out_file);;
    }

    gzclose(gzfile);
    assert_file_closed(&in_file);;

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}