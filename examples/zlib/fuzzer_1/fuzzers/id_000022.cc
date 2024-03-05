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
//<ID> 388
//<Prompt> ["inflateMark","inflateInit_","inflate","gzclearerr","inflateValidate","inflateEnd","deflateInit_","deflatePending","deflateTune","uncompress","crc32","deflateEnd"]
/*<Combination>: [long inflateMark(z_streamp strm),
    int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int inflate(z_streamp strm, int flush),
    void gzclearerr(gzFile file),
    int inflateValidate(z_streamp , int ),
    int inflateEnd(z_streamp strm),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int deflatePending(z_streamp strm, unsigned int * pending, int * bits),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    uLong crc32(uLong crc, const Bytef * buf, uInt len),
    int deflateEnd(z_streamp strm)
*/
//<score> 5, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"inflateValidate":[[1503,9,1503,14,0,0,4,0],[1503,18,1503,29,0,0,4,0]]},"library_calls":["deflateInit_","inflateInit_","deflateEnd","deflatePending","deflateTune","uncompress","inflateMark","inflateValidate","crc32","crc32","inflateEnd","deflateEnd"],"critical_calls":["deflateInit_","inflateInit_","deflatePending","deflateTune","uncompress","inflateMark","inflateValidate","crc32","crc32","inflateEnd","deflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // Step 4: Create the input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for input file
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Define the output file name
    const char* out_file_name = "output_file";
    
    // Step 7: Create the output file stream
    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get the file descriptor for output file
    int out_fd = fuzz_fileno(out_file);

    // Initialize zlib structures for compression and decompression
    z_stream deflate_strm;
    z_stream inflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    if (deflateInit_(&deflate_strm, fuzz_int32_t_2, fuzz_str_1, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize inflate stream
    if (inflateInit_(&inflate_strm, fuzz_str_3, sizeof(z_stream)) != Z_OK) {
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set input and output buffers for deflate and inflate streams
    const size_t BUF_SIZE = 4096;
    Bytef deflate_in_buf[BUF_SIZE];
    Bytef deflate_out_buf[BUF_SIZE];
    Bytef inflate_in_buf[BUF_SIZE];
    Bytef inflate_out_buf[BUF_SIZE];
    
    // Step 1: Call deflatePending() to get the pending and bits
    unsigned int deflate_pending;
    int deflate_bits;
    deflatePending(&deflate_strm, &deflate_pending, &deflate_bits);
    
    // Step 2: Call deflateTune() to optimize the compression parameters
    deflateTune(&deflate_strm, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7);
    
    // Step 3: Call uncompress() to decompress the input data
    uLongf dest_len = BUF_SIZE;
    uncompress(inflate_out_buf, &dest_len, data, size);
    
    // Step 8: Call inflateMark() to set the current position of the input stream
    inflateMark(&inflate_strm);
    
    // Step 9: Call inflateValidate() to validate the compressed data
    int result = inflateValidate(&inflate_strm, fuzz_int32_t_8);
    
    // Step 10: Call crc32() to calculate the CRC-32 checksum of the input data
    uLong crc = crc32(fuzz_uint64_t_9, Z_NULL, 0);
    crc = crc32(crc, data, size);
    
    // Clean up resources
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}