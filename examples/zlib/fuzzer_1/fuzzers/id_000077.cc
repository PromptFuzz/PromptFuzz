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
//<ID> 1492
//<Prompt> ["deflateReset","deflateBound","gzbuffer","gzfread","gzflush","gzfwrite"]
/*<Combination>: [int deflateReset(z_streamp strm),
    uLong deflateBound(z_streamp strm, uLong sourceLen),
    int gzbuffer(gzFile file, unsigned int size),
    z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file),
    int gzflush(gzFile file, int flush),
    z_size_t gzfwrite(voidpc buf, z_size_t size, z_size_t nitems, gzFile file)
*/
//<score> 32, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"gzfread":[[382,9,382,21,0,0,4,0]],"gzflush":[[532,9,532,21,0,0,4,0]],"gzfwrite":[[267,9,267,21,0,0,4,0]]},"library_calls":["deflateInit_","deflateEnd","deflate","deflateEnd","deflateEnd","deflateEnd","deflateReset","deflateBound","gzbuffer","gzfread","gzflush","gzfwrite"],"critical_calls":["deflateInit_","deflate","deflateEnd","deflateReset","deflateBound","gzbuffer","gzfread","gzflush","gzfwrite"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Provide the input file name
    const char *input_file = "input_file";
    
    // Step 4: Create a FILE pointer to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 6: Provide the output file name
    const char *output_file = "output_file";
    
    // Step 1: Event - Compress the input data using zlib APIs
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    unsigned char in[8192];
    unsigned char out[8192];

    do {
        strm.avail_in = fread(in, 1, sizeof(in), in_file);
        if (ferror(in_file)) {
            deflateEnd(&strm);
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }

        strm.next_in = in;

        do {
            strm.avail_out = sizeof(out);
            strm.next_out = out;
            ret = deflate(&strm, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&strm);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
            }

            size_t have = sizeof(out) - strm.avail_out;
            if (fwrite(out, 1, have, out_file) != have || ferror(out_file)) {
                deflateEnd(&strm);
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
            }
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);

    deflateEnd(&strm);

    // Step 2: Call other zlib APIs
    deflateReset(&strm);
    uLong bound = deflateBound(&strm, size);
    gzbuffer(NULL, fuzz_uint32_t_1);
    z_size_t fread_result = gzfread(NULL, fuzz_uint64_t_2, 10, NULL);
    gzflush(NULL, fuzz_int32_t_3);
    z_size_t fwrite_result = gzfwrite(NULL, 1, 10, NULL);

    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}