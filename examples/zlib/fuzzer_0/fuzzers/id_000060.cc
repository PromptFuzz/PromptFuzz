#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1057
//<Prompt> ["inflateInit_","deflateEnd","inflateInit2_","gzopen","inflate","inflateEnd"]
/*<Combination>: [int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int deflateEnd(z_streamp strm),
    int inflateInit2_(z_streamp strm, int windowBits, const char * version, int stream_size),
    gzFile gzopen(const char * , const char * ),
    int inflate(z_streamp strm, int flush),
    int inflateEnd(z_streamp strm)
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["inflateInit_","gzopen","gzsetparams","gzread","inflate","inflateEnd","gzclose"],"critical_calls":["inflateInit_","gzopen","gzsetparams","gzread","inflate","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    z_stream strm;
    int ret;
    
    // Step 4: Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Initialize the zlib stream for decompression
    ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // Handle error
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 3: Set input file for zlib stream
    gzFile input_file = gzopen("input_file", "rb");
    gzsetparams(input_file, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // Step 6: Use file names directly as strings
    const char* input_file_name = "input_file";
    const char* output_file_name = "output_file";
    
    // Step 2: Perform decompression
    while (true) {
        unsigned char inbuf[1024];
        unsigned char outbuf[1024];
        int bytesRead = gzread(input_file, inbuf, sizeof(inbuf));
        if (bytesRead < 0) {
            // Read error
            break;
        }
        
        if (bytesRead == 0) {
            // End of input file
            break;
        }
        
        strm.avail_in = bytesRead;
        strm.next_in = inbuf;
        
        do {
            strm.avail_out = sizeof(outbuf);
            strm.next_out = outbuf;
            
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                // Handle error
                break;
            }
            
            int bytesWritten = sizeof(outbuf) - strm.avail_out;
            fwrite(outbuf, 1, bytesWritten, out_file);
        } while (strm.avail_out == 0);
    }
    
    // Step 7: Release resources
    inflateEnd(&strm);
    gzclose(input_file);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}