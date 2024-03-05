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
//<ID> 467
//<Prompt> ["gzgetc","deflateInit2_","gzfwrite","deflateParams","gzclose_w","deflateSetHeader","deflate","inflateCodesUsed","deflateEnd"]
/*<Combination>: [int gzgetc(gzFile file),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    z_size_t gzfwrite(voidpc buf, z_size_t size, z_size_t nitems, gzFile file),
    int deflateParams(z_streamp strm, int level, int strategy),
    int gzclose_w(gzFile file),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    int deflate(z_streamp strm, int flush),
    unsigned long inflateCodesUsed(z_streamp ),
    int deflateEnd(z_streamp strm)
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"deflate":[[1183,10,1183,29,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","deflateInit2_","gzclose","gzclose","deflateSetHeader","deflateEnd","gzclose","gzclose","inflateCodesUsed","deflate","deflateEnd","gzclose","gzclose","gzfwrite","gzeof","deflate","deflateEnd","gzclose","gzclose","gzfwrite","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","deflateInit2_","deflateSetHeader","inflateCodesUsed","deflate","gzfwrite","gzeof","deflate","gzfwrite","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size){
	if(f_size<33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 7: Create a FILE pointer for writing the output data
    FILE *out_file = fopen(output_file, "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Create zlib variables
    gzFile gz_input = gzdopen(in_fd, "rb");
    gzFile gz_output = gzdopen(out_fd, "wb");
    if (gz_input == NULL || gz_output == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Declare zlib variables
    z_stream strm;
    gz_headerp head = NULL;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Initialize zlib
    int ret = deflateInit2_(&strm, fuzz_int32_t_2, fuzz_int32_t_3, 15 + 16, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_input);
        gzclose(gz_output);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set zlib header
    ret = deflateSetHeader(&strm, head);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_input);
        gzclose(gz_output);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Deflate the input data and write to output file
    unsigned char in_buffer[1024];
    unsigned char out_buffer[1024];
    strm.avail_in = 0;
    strm.next_in = in_buffer;
    strm.avail_out = sizeof(out_buffer);
    strm.next_out = out_buffer;
    do {
        // Step 1: Use gzgetc and inflateCodesUsed
        int c = gzgetc(gz_input);
        unsigned long codes_used = inflateCodesUsed(&strm);

        // Step 2: Use deflate and gzfwrite
        ret = deflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK) {
            deflateEnd(&strm);
            gzclose(gz_input);
            gzclose(gz_output);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
        gzfwrite(out_buffer, sizeof(unsigned char), sizeof(out_buffer), gz_output);

        // Step 3: Check if input and output are fully consumed
        if (strm.avail_in == 0 && strm.avail_out == 0) {
            strm.next_in = in_buffer;
            strm.avail_in = fread(in_buffer, sizeof(unsigned char), sizeof(in_buffer), in_file);
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
    } while (!gzeof(gz_input));

    // Flush the compressed data
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&strm);
        gzclose(gz_input);
        gzclose(gz_output);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    gzfwrite(out_buffer, sizeof(unsigned char), sizeof(out_buffer) - strm.avail_out, gz_output);

    // Clean up resources
    deflateEnd(&strm);
    gzclose(gz_input);
    gzclose(gz_output);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}