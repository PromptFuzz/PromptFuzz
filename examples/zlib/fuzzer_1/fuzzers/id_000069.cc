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
//<ID> 1177
//<Prompt> ["crc32_z","gzdopen","gzseek","gzflush","uncompress2","gzclose","inflateInit_","inflateSetDictionary","gzputc","inflateEnd"]
/*<Combination>: [uLong crc32_z(uLong crc, const Bytef * buf, z_size_t len),
    gzFile gzdopen(int fd, const char * mode),
    off_t gzseek(gzFile , off_t , int ),
    int gzflush(gzFile file, int flush),
    int uncompress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong * sourceLen),
    int gzclose(gzFile file),
    int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int gzputc(gzFile file, int c),
    int inflateEnd(z_streamp strm)
*/
//<score> 14.666667, nr_unique_branch: 6
//<Quality> {"density":11,"unique_branches":{"gz_open":[[125,13,125,21,0,0,4,0],[226,12,226,35,0,0,4,1],[241,9,241,33,0,0,4,0]],"gzclose_w":[[616,9,616,39,0,0,4,0],[627,9,627,31,0,0,4,0]],"gz_comp":[[109,21,109,29,0,0,4,0]]},"library_calls":["crc32","inflateInit_","inflateSetDictionary","inflate","crc32_z","inflateEnd","gzdopen","gzflush","gzclose","gzdopen","gzputc","gzputc","gzputc","gzputc","gzclose"],"critical_calls":["crc32","inflateInit_","inflateSetDictionary","inflate","crc32_z","inflateEnd","gzdopen","gzflush","gzclose","gzdopen","gzputc","gzputc","gzputc","gzputc","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file stream
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Declare zlib variables
    z_stream strm;
    Bytef in[1024];
    Bytef out[1024];
    uLong crc = crc32(fuzz_uint64_t_1, Z_NULL, 0);
    
    // Initialize zlib
    if (inflateInit_(&strm, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set dictionary for inflation
    Bytef dictionary[] = "dictionary";
    uInt dictLength = strlen((const char*)dictionary);
    inflateSetDictionary(&strm, fuzz_str_3, static_cast<uInt>(fuzz_str_sz_3));
    
    // Read and decompress input data
    while (1) {
        ssize_t read_size = fread(in, sizeof(Bytef), sizeof(in), in_file);
        if (read_size <= 0) {
            break;
        }
        
        strm.next_in = in;
        strm.avail_in = read_size;
        
        do {
            strm.next_out = out;
            strm.avail_out = sizeof(out);
            
            int ret = inflate(&strm, fuzz_int32_t_4);
            if (ret == Z_STREAM_END) {
                break;
            } else if (ret != Z_OK) {
                break;
            }
            
            // Write decompressed data to output file
            fwrite(out, sizeof(Bytef), sizeof(out) - strm.avail_out, out_file);
            
            // Update CRC32 value
            crc = crc32_z(crc, out, sizeof(out) - strm.avail_out);
        } while (strm.avail_out == 0);
    }
    
    // Finalize zlib
    inflateEnd(&strm);
    
    // Flush and close output file
    gzFile gz_out = gzdopen(out_fd, fuzz_str_5);
    gzflush(gz_out, fuzz_int32_t_6);
    gzclose(gz_out);
    
    // Write CRC32 value to output file
    gz_out = gzdopen(out_fd, fuzz_str_7);
    gzputc(gz_out, (int) (crc >> 24) & 0xff);
    gzputc(gz_out, (int) (crc >> 16) & 0xff);
    gzputc(gz_out, (int) (crc >> 8) & 0xff);
    gzputc(gz_out, (int) crc & 0xff);
    gzclose(gz_out);
    
    // Close input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}