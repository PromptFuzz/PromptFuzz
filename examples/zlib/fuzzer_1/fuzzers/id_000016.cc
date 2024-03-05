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
//<ID> 295
//<Prompt> ["deflateParams","deflateSetHeader","inflateCodesUsed","deflateInit2_","inflateBackEnd","get_crc_table"]
/*<Combination>: [int deflateParams(z_streamp strm, int level, int strategy),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    unsigned long inflateCodesUsed(z_streamp ),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    int inflateBackEnd(z_streamp strm),
    const z_crc_t *get_crc_table()
*/
//<score> 5.3333335, nr_unique_branch: 18
//<Quality> {"density":8,"unique_branches":{"deflate":[[1041,13,1041,32,0,0,4,1],[1075,17,1075,43,0,0,4,1],[1079,17,1079,32,0,0,4,1],[1086,9,1086,33,0,0,4,0],[1087,13,1087,39,0,0,4,1],[1113,9,1113,32,0,0,4,0],[1114,13,1114,38,0,0,4,1],[1135,9,1135,35,0,0,4,0],[1136,13,1136,41,0,0,4,1],[1156,9,1156,32,0,0,4,0],[1157,13,1157,28,0,0,4,1],[1173,13,1173,28,0,0,4,1],[285,64,285,65,40,0,4,1],[285,64,285,65,40,0,4,1],[285,64,285,65,40,0,4,0],[285,64,285,65,40,0,4,0],[285,64,285,65,40,0,4,0],[285,64,285,65,45,0,4,0]]},"library_calls":["deflateInit2_","deflateParams","deflateSetHeader","deflate","deflateEnd","deflateEnd","inflateCodesUsed","deflateEnd"],"critical_calls":["deflateInit2_","deflateParams","deflateSetHeader","deflate","inflateCodesUsed","deflateEnd"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size)
{
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





    // Step 4: Open input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == NULL || out_file == NULL) {
        // Handle file opening error
        if (in_file != NULL)
            assert_file_closed(&in_file);;
        if (out_file != NULL)
            assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define file names
    const char *in_file_name = "input_file";
    const char *out_file_name = "output_file";

    // Step 2: Call zlib APIs
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // Initialize zlib stream for deflate
    int ret = deflateInit2_(&strm, fuzz_int32_t_2, fuzz_int32_t_3, 15 + 16, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        // Handle initialization error
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

    // Set deflate parameters
    deflateParams(&strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Set deflate header
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 0;
    header.time = 0;
    header.xflags = 0;
    header.hcrc = 0;
    header.done = 0;
    deflateSetHeader(&strm, &header);

    // Perform deflate operation
    char in_buffer[1024];
    char out_buffer[1024];
    size_t bytes_read, bytes_written;
    do {
        bytes_read = fread(in_buffer, 1, sizeof(in_buffer), in_file);
        if (bytes_read > 0) {
            strm.next_in = (Bytef *)in_buffer;
            strm.avail_in = bytes_read;

            do {
                strm.next_out = (Bytef *)out_buffer;
                strm.avail_out = sizeof(out_buffer);

                ret = deflate(&strm, Z_FINISH);
                if (ret == Z_STREAM_ERROR) {
                    // Handle deflate error
                    deflateEnd(&strm);
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

                bytes_written = fwrite(out_buffer, 1, sizeof(out_buffer) - strm.avail_out, out_file);
                if (bytes_written == 0) {
                    // Handle write error
                    deflateEnd(&strm);
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
            } while (strm.avail_out == 0);
        }
    } while (bytes_read > 0);

    // Get number of codes used by inflate
    unsigned long codes_used = inflateCodesUsed(&strm);

    // Step 7: Cleanup and release resources
    deflateEnd(&strm);
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