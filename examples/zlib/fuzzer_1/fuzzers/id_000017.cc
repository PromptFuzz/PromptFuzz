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
//<ID> 299
//<Prompt> ["deflateParams","deflateSetHeader","inflateCodesUsed","deflateInit2_","inflateBackEnd","get_crc_table"]
/*<Combination>: [int deflateParams(z_streamp strm, int level, int strategy),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    unsigned long inflateCodesUsed(z_streamp ),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    int inflateBackEnd(z_streamp strm),
    const z_crc_t *get_crc_table()
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"deflate":[[285,64,285,65,45,0,4,1],[285,64,285,65,45,0,4,1],[285,64,285,65,45,0,4,1]]},"library_calls":["deflateInit2_","deflateParams","deflateSetHeader","get_crc_table","deflate","inflateCodesUsed","deflateEnd"],"critical_calls":["deflateInit2_","deflateParams","deflateSetHeader","get_crc_table","deflate","inflateCodesUsed","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE pointer to write output data
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
    unsigned char *input_buffer = (unsigned char *)malloc(size);
    size_t read_size = fread(input_buffer, 1, size, in_file);

    // Initialize zlib stream for compression
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        free(input_buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set deflate parameters
    deflateParams(&strm, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Set the gzip header
    gz_header head;
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 0;
    head.extra = NULL;
    head.extra_len = 0;
    head.extra_max = 0;
    head.name = NULL;
    head.name_max = 0;
    head.comment = NULL;
    head.comm_max = 0;
    head.hcrc = 0;
    head.done = 0;
    deflateSetHeader(&strm, &head);

    // Get the CRC table
    const z_crc_t *crc_table = get_crc_table();

    // Compress the input data
    unsigned char output_buffer[1024];
    strm.avail_in = read_size;
    strm.next_in = input_buffer;
    strm.avail_out = sizeof(output_buffer);
    strm.next_out = output_buffer;

    do {
        ret = deflate(&strm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        }

        if (strm.avail_out == 0) {
            // Write the compressed data to the file
            write(out_fd, output_buffer, sizeof(output_buffer));

            strm.avail_out = sizeof(output_buffer);
            strm.next_out = output_buffer;
        }
    } while (ret == Z_OK);

    // Write the remaining compressed data to the file
    size_t write_size = sizeof(output_buffer) - strm.avail_out;
    write(out_fd, output_buffer, write_size);

    // Get the number of codes used during decompression
    unsigned long codes_used = inflateCodesUsed(&strm);

    // Clean up zlib stream
    deflateEnd(&strm);

    // Clean up resources
    free(input_buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}