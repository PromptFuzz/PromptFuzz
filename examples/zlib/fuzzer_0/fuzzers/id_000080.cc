#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1540
//<Prompt> ["inflateInit_","gzrewind","inflatePrime","gzopen","gzdirect","inflateInit_","inflateGetHeader","inflateEnd","gzclose"]
/*<Combination>: [int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int gzrewind(gzFile file),
    int inflatePrime(z_streamp strm, int bits, int value),
    gzFile gzopen(const char * , const char * ),
    int gzdirect(gzFile file),
    int inflateInit_(z_streamp strm, const char * version, int stream_size),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    int inflateEnd(z_streamp strm),
    int gzclose(gzFile file)
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["inflateInit_","gzopen","inflateEnd","gzopen","gzclose","inflateEnd","gzrewind","inflatePrime","gzdirect","inflateGetHeader","gzeof","gzread","gzclose","gzclose","inflateEnd","inflate","gzclose","gzclose","inflateEnd","gzclose","gzclose","inflateEnd","gzclose","gzclose","inflateEnd"],"critical_calls":["inflateInit_","gzopen","gzopen","gzrewind","inflatePrime","gzdirect","inflateGetHeader","gzeof","gzread","inflate","gzclose","gzclose","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE pointers for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Initialize zlib structs and variables
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        std::cerr << "Error initializing inflate" << std::endl;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    gzFile gz_in = gzopen("input_file", "rb");
    
    // Step 6: Use file names as strings
    if (gz_in == NULL) {
        std::cerr << "Error opening input file" << std::endl;
        inflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    gzFile gz_out = gzopen("output_file", "wb");
    
    if (gz_out == NULL) {
        std::cerr << "Error opening output file" << std::endl;
        gzclose(gz_in);
        inflateEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: Call zlib APIs
    gzrewind(gz_in);
    inflatePrime(&strm, 16, 0);
    gzdirect(gz_in);
    inflateGetHeader(&strm, Z_NULL);
    
    // Step 3: Consume input data
    while (!gzeof(gz_in)) {
        unsigned char buffer[1024];
        int len = gzread(gz_in, buffer, sizeof(buffer));
        
        if (len < 0) {
            std::cerr << "Error reading input file" << std::endl;
            gzclose(gz_in);
            gzclose(gz_out);
            inflateEnd(&strm);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        if (inflate(&strm, Z_NO_FLUSH) != Z_OK) {
            std::cerr << "Error inflating data" << std::endl;
            gzclose(gz_in);
            gzclose(gz_out);
            inflateEnd(&strm);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        
        if (strm.avail_out == 0) {
            if (fwrite(strm.next_out, 1, 1024, out_file) != 1024) {
                std::cerr << "Error writing output file" << std::endl;
                gzclose(gz_in);
                gzclose(gz_out);
                inflateEnd(&strm);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
            }
            
            strm.next_out = buffer;
            strm.avail_out = sizeof(buffer);
        }
    }
    
    // Step 7: Clean up resources
    gzclose(gz_in);
    gzclose(gz_out);
    inflateEnd(&strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}