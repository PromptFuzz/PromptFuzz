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
//<ID> 1303
//<Prompt> ["inflatePrime","deflateBound","gzeof","inflateBack","gzoffset64","gztell","inflateCopy","inflateReset2","gzopen","gzputs","deflateTune","inflateBackEnd","deflateResetKeep","gzclose"]
/*<Combination>: [int inflatePrime(z_streamp strm, int bits, int value),
    uLong deflateBound(z_streamp strm, uLong sourceLen),
    int gzeof(gzFile file),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    off64_t gzoffset64(gzFile ),
    off_t gztell(gzFile ),
    int inflateCopy(z_streamp dest, z_streamp source),
    int inflateReset2(z_streamp strm, int windowBits),
    gzFile gzopen(const char * , const char * ),
    int gzputs(gzFile file, const char * s),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int inflateBackEnd(z_streamp strm),
    int deflateResetKeep(z_streamp ),
    int gzclose(gzFile file)
*/
//<score> 44, nr_unique_branch: 4
//<Quality> {"density":11,"unique_branches":{"gztell64":[[427,35,427,58,0,0,4,0]],"gzoffset64":[[451,35,451,58,0,0,4,0]],"gzeof":[[479,9,479,31,0,0,4,0],[479,35,479,58,0,0,4,0]]},"library_calls":["inflatePrime","deflateBound","gzeof","inflateBack","gzoffset64","gztell","inflateCopy","inflateReset2","gzopen","gzputs","deflateTune","inflateBackEnd","deflateResetKeep","gzclose"],"critical_calls":["inflatePrime","deflateBound","gzeof","inflateBack","gzoffset64","gztell","inflateCopy","inflateReset2","gzopen","gzputs","deflateTune","inflateBackEnd","deflateResetKeep","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create file pointers for input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Define file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Use zlib APIs to achieve a specific event
    z_stream strm;
    int ret;
    int bits = 8;
    int value = 0;
    uLong sourceLen = size;
    int windowBits = 15; // Default windowBits value for inflate

    // Step 2: Call zlib APIs
    ret = inflatePrime(&strm, bits, fuzz_int32_t_1);
    ret = deflateBound(&strm, sourceLen);
    ret = gzeof((gzFile)in_file);
    ret = inflateBack(&strm, (in_func)in_file, (void *)in_fd, (out_func)out_file, (void *)out_fd);
    off64_t offset = gzoffset64((gzFile)in_file);
    off_t tell = gztell((gzFile)in_file);
    ret = inflateCopy(&strm, &strm);
    ret = inflateReset2(&strm, fuzz_int32_t_2);
    gzFile gzfile = gzopen(input_file, fuzz_str_3);
    ret = gzputs(gzfile, fuzz_str_4);
    ret = deflateTune(&strm, fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8);
    ret = inflateBackEnd(&strm);
    ret = deflateResetKeep(&strm);
    ret = gzclose(gzfile);

    // Step 7: Release allocated resources
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