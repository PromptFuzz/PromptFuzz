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
//<ID> 1426
//<Prompt> ["deflateCopy","compress2","gzopen","gzopen64","gzseek","gzclose","deflate","gzbuffer","deflateInit2_","deflateParams","crc32_z"]
/*<Combination>: [int deflateCopy(z_streamp dest, z_streamp source),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    gzFile gzopen(const char * , const char * ),
    gzFile gzopen64(const char * , const char * ),
    off_t gzseek(gzFile , off_t , int ),
    int gzclose(gzFile file),
    int deflate(z_streamp strm, int flush),
    int gzbuffer(gzFile file, unsigned int size),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    int deflateParams(z_streamp strm, int level, int strategy),
    uLong crc32_z(uLong crc, const Bytef * buf, z_size_t len)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"deflate":[[1055,17,1055,32,0,0,4,0]]},"library_calls":["inflateInit2_","deflateInit2_","inflate","deflate","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","inflate","deflate","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Specify the input file name
    const char *input_file = "input_file";

    // Step 7: Create a FILE * variable to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 8: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 9: Specify the output file name
    const char *output_file = "output_file";

    // Step 10: Create z_stream variables
    z_stream strm_in;
    z_stream strm_out;

    // Step 11: Initialize the z_stream variables
    strm_in.zalloc = Z_NULL;
    strm_in.zfree = Z_NULL;
    strm_in.opaque = Z_NULL;
    strm_in.avail_in = 0;
    strm_in.next_in = Z_NULL;
    int ret = inflateInit2(&strm_in, 15 + 32); // Use gzip decoding

    strm_out.zalloc = Z_NULL;
    strm_out.zfree = Z_NULL;
    strm_out.opaque = Z_NULL;
    strm_out.avail_in = 0;
    strm_out.next_in = Z_NULL;
    ret = deflateInit2(&strm_out, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);

    // Step 12: Read the compressed data from the input file
    fseek(in_file, 0, SEEK_END);
    size_t compressed_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    uint8_t *compressed_data = (uint8_t *)malloc(compressed_size);
    fread(compressed_data, 1, compressed_size, in_file);
    assert_file_closed(&in_file);;

    // Step 13: Set up the input data for decompression
    strm_in.avail_in = compressed_size;
    strm_in.next_in = compressed_data;

    // Step 14: Set up the output data for compression
    uint8_t *output_data = (uint8_t *)malloc(size);
    strm_out.avail_out = size;
    strm_out.next_out = output_data;

    // Step 15: Decompress the input data
    ret = inflate(&strm_in, fuzz_int32_t_1);

    // Step 16: Compress the decompressed data
    ret = deflate(&strm_out, Z_FINISH);

    // Step 17: Write the compressed data to the output file
    fwrite(output_data, 1, size - strm_out.avail_out, out_file);
    assert_file_closed(&out_file);;

    // Step 18: Free allocated memory
    inflateEnd(&strm_in);
    deflateEnd(&strm_out);
    free(compressed_data);
    free(output_data);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}