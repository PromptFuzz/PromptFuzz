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
//<ID> 1200
//<Prompt> ["crc32","inflateInit_","inflateSetDictionary","gzread","gzclose_w","inflateSetDictionary","gzdopen","gzflush","gzclose","gzputc"]
/*<Combination>: [uLong crc32(uLong crc, const Bytef * buf, uInt len),
    int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int gzclose_w(gzFile file),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    gzFile gzdopen(int fd, const char * mode),
    int gzflush(gzFile file, int flush),
    int gzclose(gzFile file),
    int gzputc(gzFile file, int c)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["gzopen","inflateInit_","gzclose","inflateEnd","gzclose","inflate","inflateEnd","gzclose","gzwrite","inflateEnd","gzclose","inflateEnd","gzflush","gzclose","gzclose"],"critical_calls":["gzopen","inflateInit_","inflate","gzwrite","inflateEnd","gzflush","gzclose"],"visited":0}
/*1. The event that can be achieved using the provided zlib APIs is decompressing a zlib-compressed data stream, reading the decompressed data, and writing it to an output file.

2. Here is an implementation of the `LLVMFuzzerTestOneInput_71` function that achieves the event using the zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0; // Error handling
    }
    
    // Create a gzFile pointer to write the output data
    gzFile out_file = gzopen("output_file", fuzz_str_1);
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
    }
    
    // Decompress the zlib-compressed data
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    if (inflateInit_(&strm, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        gzclose(out_file);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
    }
    
    const size_t BUFFER_SIZE = 1024;
    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE];
    
    strm.next_in = in_buffer;
    strm.avail_in = 0;
    strm.next_out = out_buffer;
    strm.avail_out = BUFFER_SIZE;
    
    int ret;
    do {
        if (strm.avail_in == 0) {
            strm.avail_in = fread(in_buffer, 1, BUFFER_SIZE, in_file);
            if (ferror(in_file)) {
                inflateEnd(&strm);
                assert_file_closed(&in_file);;
                gzclose(out_file);
                assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
            }
            
            if (strm.avail_in == 0) {
                break;
            }
            
            strm.next_in = in_buffer;
        }
        
        ret = inflate(&strm, fuzz_int32_t_3);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&strm);
            assert_file_closed(&in_file);;
            gzclose(out_file);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
        }
        
        if (strm.avail_out == 0 || ret == Z_STREAM_END) {
            size_t write_size = BUFFER_SIZE - strm.avail_out;
            if (gzwrite(out_file, out_buffer, write_size) != write_size) {
                inflateEnd(&strm);
                assert_file_closed(&in_file);;
                gzclose(out_file);
                assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
            }
            
            strm.next_out = out_buffer;
            strm.avail_out = BUFFER_SIZE;
        }
    } while (ret != Z_STREAM_END);
    
    inflateEnd(&strm);
    assert_file_closed(&in_file);;
    
    // Flush the remaining output data
    if (gzflush(out_file, fuzz_int32_t_4) != Z_OK) {
        gzclose(out_file);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0; // Error handling
    }
    
    gzclose(out_file);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}