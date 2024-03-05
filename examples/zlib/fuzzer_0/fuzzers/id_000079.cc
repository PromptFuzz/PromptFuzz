#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1509
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
//<score> 12, nr_unique_branch: 8
//<Quality> {"density":6,"unique_branches":{"deflateBound":[[854,9,854,32,0,0,4,1],[860,5,860,11,0,0,4,1],[863,5,863,11,0,0,4,1],[867,5,867,11,0,0,4,0],[869,13,869,32,0,0,4,1],[888,5,888,12,0,0,4,1],[893,9,893,24,0,0,4,1],[893,28,893,49,0,0,4,1]]},"library_calls":["deflateReset","deflateInit2_","deflateBound","compress2","inflateResetKeep","deflateEnd"],"critical_calls":["deflateReset","deflateInit2_","deflateBound","compress2","inflateResetKeep","deflateEnd"],"visited":0}
/*Here is the C++ program that uses the zlib library APIs to achieve the given event:

*/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Read the input data from a FILE *
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Provide the file name
    const char *in_filename = "input_file";
    
    // Step 1: Initialize the zlib stream for compression
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    deflateReset(&strm);
    deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
    
    // Step 2: Compress the input data
    uLongf compressed_size = deflateBound(&strm, size);
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    if (compressed_data == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    uLongf compressed_len = compressed_size;
    compress2(compressed_data, &compressed_len, data, size, Z_DEFAULT_COMPRESSION);
    
    // Step 3: Reset the zlib stream for decompression
    inflateResetKeep(&strm);

    // Step 7: Release allocated resources
    free(compressed_data);
    deflateEnd(&strm);
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}