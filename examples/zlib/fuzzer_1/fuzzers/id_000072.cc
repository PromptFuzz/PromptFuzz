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
//<ID> 1276
//<Prompt> ["deflateBound","deflateResetKeep","inflateMark","gzoffset","deflatePrime","compressBound","inflateSyncPoint","compress2","gztell"]
/*<Combination>: [uLong deflateBound(z_streamp strm, uLong sourceLen),
    int deflateResetKeep(z_streamp ),
    long inflateMark(z_streamp strm),
    off_t gzoffset(gzFile ),
    int deflatePrime(z_streamp strm, int bits, int value),
    uLong compressBound(uLong sourceLen),
    int inflateSyncPoint(z_streamp ),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    off_t gztell(gzFile )
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"inflate":[[1245,37,1245,54,0,0,4,1],[476,13,476,22,113,0,4,0]]},"library_calls":["deflateInit_","deflate","deflateEnd","deflateResetKeep","deflateEnd","inflateInit_","deflateEnd","inflate","inflateEnd","deflateEnd","inflateEnd","deflateEnd"],"critical_calls":["deflateInit_","deflate","deflateResetKeep","inflateInit_","inflate","inflateEnd","deflateEnd"],"visited":0}
/*Here is the C++ program that implements the event using the specified zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Read the input data from the file
    Bytef buf[1024];
    uLongf destLen = sizeof(buf);
    if (fread(buf, 1, sizeof(buf), in_file) != sizeof(buf)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize zlib stream for compression
    z_stream c_stream;
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;
    c_stream.avail_in = sizeof(buf);
    c_stream.next_in = buf;
    c_stream.avail_out = destLen;
    c_stream.next_out = buf;
    
    // Compress the data
    int ret = deflateInit(&c_stream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    ret = deflate(&c_stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&c_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Get the compressed data size
    uLong compressedSize = c_stream.total_out;
    
    // Reset the compression stream
    ret = deflateResetKeep(&c_stream);
    if (ret != Z_OK) {
        deflateEnd(&c_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize zlib stream for decompression
    z_stream d_stream;
    d_stream.zalloc = Z_NULL;
    d_stream.zfree = Z_NULL;
    d_stream.opaque = Z_NULL;
    d_stream.avail_in = compressedSize;
    d_stream.next_in = buf;
    d_stream.avail_out = sizeof(buf);
    d_stream.next_out = buf;
    
    // Decompress the data
    ret = inflateInit(&d_stream);
    if (ret != Z_OK) {
        deflateEnd(&c_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    ret = inflate(&d_stream, fuzz_int32_t_1);
    if (ret != Z_STREAM_END) {
        inflateEnd(&d_stream);
        deflateEnd(&c_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Write the decompressed data to the output file
    fwrite(buf, 1, d_stream.total_out, out_file);
    
    // Clean up
    inflateEnd(&d_stream);
    deflateEnd(&c_stream);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}