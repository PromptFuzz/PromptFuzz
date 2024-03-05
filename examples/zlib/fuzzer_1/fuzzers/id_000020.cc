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
//<ID> 370
//<Prompt> ["inflateValidate","gzerror","deflateInit_","deflatePending","deflateTune","inflateBack","crc32"]
/*<Combination>: [int inflateValidate(z_streamp , int ),
    const char *gzerror(gzFile file, int * errnum),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int deflatePending(z_streamp strm, unsigned int * pending, int * bits),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    uLong crc32(uLong crc, const Bytef * buf, uInt len)
*/
//<score> 6, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"deflateTune":[[805,9,805,32,0,0,4,1]],"inflateValidate":[[1501,9,1501,32,0,0,4,1],[1503,9,1503,14,0,0,4,1]]},"library_calls":["inflateInit_","inflate","inflateEnd","inflateValidate","inflateEnd","inflateEnd","deflateInit_","deflatePending","deflateEnd","deflateTune","deflateEnd","crc32","crc32","deflateEnd"],"critical_calls":["inflateInit_","inflate","inflateValidate","inflateEnd","deflateInit_","deflatePending","deflateTune","crc32","crc32","deflateEnd"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
	//fuzzer shim end}




    // Step 1: Event - Decompress zlib compressed data and validate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return 0;
    }
    
    strm.avail_in = size;
    strm.next_in = const_cast<Bytef*>(data);
    
    unsigned char outBuffer[4096];
    strm.avail_out = sizeof(outBuffer);
    strm.next_out = outBuffer;
    
    ret = inflate(&strm, fuzz_int32_t_1);
    if (ret != Z_STREAM_END) {
        inflateEnd(&strm);
        return 0;
    }
    
    ret = inflateValidate(&strm, fuzz_int32_t_2);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return 0;
    }
    
    inflateEnd(&strm);

    // Step 2: Event - Compress data with zlib and calculate CRC32 checksum
    z_streamp strm2 = static_cast<z_streamp>(malloc(sizeof(z_stream)));
    memset(strm2, 0, sizeof(z_stream));
    
    ret = deflateInit_(strm2, fuzz_int32_t_4, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        free(strm2);
        return 0;
    }
    
    unsigned int pending;
    int bits;
    ret = deflatePending(strm2, &pending, &bits);
    if (ret != Z_OK) {
        deflateEnd(strm2);
        free(strm2);
        return 0;
    }
    
    ret = deflateTune(strm2, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8);
    if (ret != Z_OK) {
        deflateEnd(strm2);
        free(strm2);
        return 0;
    }
    
    uLong crc = crc32(fuzz_uint64_t_9, Z_NULL, 0);
    crc = crc32(crc, const_cast<Bytef*>(data), static_cast<uInt>(size));

    // Step 3: Event - Output compressed data and CRC32 checksum
    FILE* out_file = fopen("output_file", "wb");
    fwrite(outBuffer, sizeof(char), strm2->total_out, out_file);
    fwrite(&crc, sizeof(uLong), 1, out_file);
    assert_file_closed(&out_file);;
    
    // Step 4: Release resources
    deflateEnd(strm2);
    free(strm2);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	return 0;
}