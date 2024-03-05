#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 419
//<Prompt> ["deflateCopy","compress2","gzseek","gzgetc","gzbuffer","deflateParams","crc32_z"]
/*<Combination>: [int deflateCopy(z_streamp dest, z_streamp source),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    off_t gzseek(gzFile , off_t , int ),
    int gzgetc(gzFile file),
    int gzbuffer(gzFile file, unsigned int size),
    int deflateParams(z_streamp strm, int level, int strategy),
    uLong crc32_z(uLong crc, const Bytef * buf, z_size_t len)
*/
//<score> 5.3333335, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gzgetc":[[418,9,418,31,0,0,4,0]]},"library_calls":["gzopen","deflateInit_","gzclose","deflate","deflateEnd","gzclose","gzwrite","deflateEnd","gzclose","gzseek","gzbuffer","deflateParams","crc32_z","gzclose"],"critical_calls":["gzopen","deflateInit_","deflate","gzwrite","deflateEnd","gzseek","gzbuffer","deflateParams","crc32_z","gzclose"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set the input file name
    const char *input_file = "input_file";
    
    // Step 1: Deflate the input data and write it to the output file
    gzFile out_file = gzopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Deflate the input data using zlib API
    z_stream strm;
    Bytef input_buffer[4096];
    Bytef output_buffer[4096];
    int ret;
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = fread(input_buffer, 1, sizeof(input_buffer), in_file);
    strm.next_in = input_buffer;
    
    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        gzclose(out_file);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    do {
        strm.avail_out = sizeof(output_buffer);
        strm.next_out = output_buffer;
        ret = deflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&strm);
            gzclose(out_file);
            assert_file_closed(&in_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        size_t bytes_written = sizeof(output_buffer) - strm.avail_out;
        gzwrite(out_file, output_buffer, bytes_written);
    } while (strm.avail_out == 0);
    
    ret = deflateEnd(&strm);
    if (ret != Z_OK) {
        gzclose(out_file);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Use the input and output files with zlib APIs
    off_t current_pos = gzseek(out_file, 0, SEEK_CUR);
    int c = gzgetc(out_file);
    unsigned int buffer_size = 1024;
    int buffer_result = gzbuffer(out_file, buffer_size);
    int params_result = deflateParams(&strm, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    uLong crc_result = crc32_z(0, (const Bytef *)data, size);
    
    // Step 7: Release resources
    gzclose(out_file);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}