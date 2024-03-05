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
//<ID> 546
//<Prompt> ["inflateBack","adler32_combine64","inflateInit2_","deflateSetHeader","gzfread"]
/*<Combination>: [int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    uLong adler32_combine64(uLong , uLong , off64_t ),
    int inflateInit2_(z_streamp strm, int windowBits, const char * version, int stream_size),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["inflateInit2_","deflateSetHeader","inflate","inflateEnd","inflateEnd"],"critical_calls":["inflateInit2_","deflateSetHeader","inflate","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Step 4: Create FILE pointers for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    if (!in_file || !out_file) {
        // Error handling if file pointer creation failed
        std::cerr << "Error opening file" << std::endl;
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 1;
    }

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Call zlib APIs
    z_stream strm;
    inflateInit2_(&strm, 15 | 32, fuzz_str_1, sizeof(z_stream));

    gz_header header;
    deflateSetHeader(&strm, &header);

    const int BUFFER_SIZE = 8192;
    unsigned char in_buffer[BUFFER_SIZE];
    unsigned char out_buffer[BUFFER_SIZE];

    do {
        strm.next_in = in_buffer;
        strm.avail_in = fread(in_buffer, 1, BUFFER_SIZE, in_file);

        if (strm.avail_in == 0)
            break;

        do {
            strm.next_out = out_buffer;
            strm.avail_out = BUFFER_SIZE;

            int ret = inflate(&strm, fuzz_int32_t_2);
            if (ret == Z_STREAM_END)
                break;

            if (ret != Z_OK) {
                // Error handling if inflate failed
                std::cerr << "Error inflating data" << std::endl;
                inflateEnd(&strm);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 1;
            }

            fwrite(out_buffer, 1, BUFFER_SIZE - strm.avail_out, out_file);
        } while (strm.avail_out == 0);
    } while (true);

    // Step 7: Release resources
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