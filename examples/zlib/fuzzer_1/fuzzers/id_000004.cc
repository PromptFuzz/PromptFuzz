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
//<ID> 42
//<Prompt> ["gzsetparams","deflatePrime","deflatePending","gzputs","gzfwrite"]
/*<Combination>: [int gzsetparams(gzFile file, int level, int strategy),
    int deflatePrime(z_streamp strm, int bits, int value),
    int deflatePending(z_streamp strm, unsigned int * pending, int * bits),
    int gzputs(gzFile file, const char * s),
    z_size_t gzfwrite(voidpc buf, z_size_t size, z_size_t nitems, gzFile file)
*/
//<score> 5, nr_unique_branch: 11
//<Quality> {"density":5,"unique_branches":{"deflatePending":[[718,9,718,32,0,0,4,1],[719,9,719,26,0,0,4,0],[721,9,721,23,0,0,4,0]],"deflateInit2_":[[414,9,414,23,0,0,4,0],[416,13,416,29,0,0,4,1]],"deflatePrime":[[731,9,731,32,0,0,4,1],[738,9,738,17,0,0,4,1],[738,21,738,30,0,0,4,1],[739,9,739,60,0,0,4,1],[744,13,744,23,0,0,4,0],[751,14,751,18,0,0,4,1]]},"library_calls":["gzdopen","gzsetparams","deflateInit2_","deflatePrime","deflatePending","gzputs","gzfwrite","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzsetparams","deflateInit2_","deflatePrime","deflatePending","gzputs","gzfwrite","deflateEnd","gzclose"],"visited":3}
/**/



extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    // Step 4: Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Achieve the event using zlib APIs
    gzFile gz_file = gzdopen(in_fd, fuzz_str_1);
    gzsetparams(gz_file, fuzz_int32_t_2, fuzz_int32_t_3);
    
    z_stream strm;
    strm.next_in = NULL;
    strm.avail_in = 0;
    strm.next_out = NULL;
    strm.avail_out = 0;
    deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    deflatePrime(&strm, fuzz_int32_t_4, fuzz_int32_t_5);
    
    unsigned int pending;
    int bits;
    deflatePending(&strm, &pending, &bits);
    
    char *str = "This is a test";
    gzputs(gz_file, fuzz_str_6);
    
    z_size_t size_written = gzfwrite(fuzz_str_7, sizeof(char), static_cast<z_size_t>(fuzz_str_sz_7), gz_file);
    
    // Step 7: Free resources
    deflateEnd(&strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}