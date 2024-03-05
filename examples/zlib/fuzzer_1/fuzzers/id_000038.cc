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
//<ID> 638
//<Prompt> ["gzopen","gzputc","inflateInit2_","inflateGetDictionary","inflatePrime","inflateCodesUsed","gzclose","inflateEnd"]
/*<Combination>: [gzFile gzopen(const char * , const char * ),
    int gzputc(gzFile file, int c),
    int inflateInit2_(z_streamp strm, int windowBits, const char * version, int stream_size),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int inflatePrime(z_streamp strm, int bits, int value),
    unsigned long inflateCodesUsed(z_streamp ),
    int gzclose(gzFile file),
    int inflateEnd(z_streamp strm)
*/
//<score> 5, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"inflateGetDictionary":[[1287,9,1287,21,0,0,4,0],[1287,25,1287,45,0,0,4,0]]},"library_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateGetDictionary","inflateCodesUsed","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Open input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Initialize zlib stream
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    if (inflateInit2_(&strm, 15 + 32, fuzz_str_1, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read input file
    const int BUFFER_SIZE = 1024;
    Bytef buffer[BUFFER_SIZE];
    int bytes_read;
    do {
        bytes_read = fread(buffer, sizeof(Bytef), BUFFER_SIZE, in_file);
        if (bytes_read > 0) {
            strm.next_in = buffer;
            strm.avail_in = bytes_read;
            
            // Decompress input data
            do {
                Bytef out_buffer[BUFFER_SIZE];
                strm.next_out = out_buffer;
                strm.avail_out = BUFFER_SIZE;
                
                int ret = inflate(&strm, fuzz_int32_t_2);
                if (ret == Z_STREAM_END) {
                    fwrite(out_buffer, sizeof(Bytef), BUFFER_SIZE - strm.avail_out, out_file);
                    break;
                } else if (ret != Z_OK) {
                    break;
                }
                
                fwrite(out_buffer, sizeof(Bytef), BUFFER_SIZE - strm.avail_out, out_file);
            } while (strm.avail_out == 0);
        }
    } while (bytes_read > 0);
    
    // Get dictionary used by zlib
    Bytef dict[32768];
    uInt dict_length = sizeof(dict);
    inflateGetDictionary(&strm, dict, &dict_length);
    
    // Get number of codes used by zlib
    unsigned long codes_used = inflateCodesUsed(&strm);
    
    // Clean up
    inflateEnd(&strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}