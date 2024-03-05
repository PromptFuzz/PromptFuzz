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
//<ID> 765
//<Prompt> ["inflateSyncPoint","deflateInit_","deflatePending","deflateTune","inflateValidate","inflateBackInit_","crc32","inflateUndermine","inflateEnd","deflateEnd"]
/*<Combination>: [int inflateSyncPoint(z_streamp ),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int deflatePending(z_streamp strm, unsigned int * pending, int * bits),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int inflateValidate(z_streamp , int ),
    int inflateBackInit_(z_streamp strm, int windowBits, unsigned char * window, const char * version, int stream_size),
    uLong crc32(uLong crc, const Bytef * buf, uInt len),
    int inflateUndermine(z_streamp , int ),
    int inflateEnd(z_streamp strm),
    int deflateEnd(z_streamp strm)
*/
//<score> 5.5, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"deflatePending":[[719,9,719,26,0,0,4,1],[721,9,721,23,0,0,4,1]]},"library_calls":["inflateSyncPoint","deflateInit_","zlibVersion","deflatePending","deflateTune","inflateValidate","inflateBackInit_","zlibVersion","crc32","inflateUndermine","inflateEnd","deflateEnd"],"critical_calls":["inflateSyncPoint","deflateInit_","zlibVersion","deflatePending","deflateTune","inflateValidate","inflateBackInit_","zlibVersion","crc32","inflateUndermine","inflateEnd","deflateEnd"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create FILE variables for reading and writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Achieve event using zlib APIs
    z_stream strm;
    int ret;

    // Step 2: Call zlib APIs
    ret = inflateSyncPoint(&strm);
    ret = deflateInit_(&strm, fuzz_int32_t_1, zlibVersion(), sizeof(z_stream));
    ret = deflatePending(&strm, NULL, NULL);
    ret = deflateTune(&strm, fuzz_int32_t_2, fuzz_int32_t_3, fuzz_int32_t_4, fuzz_int32_t_5);
    ret = inflateValidate(&strm, fuzz_int32_t_6);
    ret = inflateBackInit_(&strm, 15, NULL, zlibVersion(), sizeof(z_stream));
    uLong crc = crc32(fuzz_uint64_t_7, NULL, 0);
    ret = inflateUndermine(&strm, fuzz_int32_t_8);
    ret = inflateEnd(&strm);
    ret = deflateEnd(&strm);

    // Step 7: Release resources
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