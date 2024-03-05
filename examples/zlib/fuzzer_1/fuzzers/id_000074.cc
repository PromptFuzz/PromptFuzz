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
//<ID> 1398
//<Prompt> ["inflateReset","gzdirect","deflateResetKeep","gzputs","gztell","uncompress","deflatePrime","inflateMark","inflateBackEnd"]
/*<Combination>: [int inflateReset(z_streamp strm),
    int gzdirect(gzFile file),
    int deflateResetKeep(z_streamp ),
    int gzputs(gzFile file, const char * s),
    off_t gztell(gzFile ),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int deflatePrime(z_streamp strm, int bits, int value),
    long inflateMark(z_streamp strm),
    int inflateBackEnd(z_streamp strm)
*/
//<score> 7, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"inflateBackEnd":[[622,27,622,48,0,0,4,1],[622,52,622,79,0,0,4,1]]},"library_calls":["inflateInit_","inflateReset","gzdopen","gzdirect","deflateResetKeep","gzputs","gztell","uncompress","deflatePrime","inflateMark","inflateBackEnd","inflateEnd","gzclose"],"critical_calls":["inflateInit_","inflateReset","gzdopen","gzdirect","deflateResetKeep","gzputs","gztell","uncompress","deflatePrime","inflateMark","inflateBackEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Step 4: Create input and output file streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors for input and output streams
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Perform zlib operations on input data
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    inflateInit(&strm);
    inflateReset(&strm);
    gzFile gzfile = gzdopen(in_fd, fuzz_str_1);
    gzdirect(gzfile);
    deflateResetKeep(&strm);
    gzputs(gzfile, fuzz_str_2);
    gztell(gzfile);
    unsigned char *dest = (unsigned char *)malloc(size * 2);
    uLongf destLen = size * 2;
    uncompress(dest, &destLen, (const Bytef *)data, (uLong)size);
    deflatePrime(&strm, fuzz_int32_t_3, fuzz_int32_t_4);
    inflateMark(&strm);
    inflateBackEnd(&strm);

    // Step 7: Release allocated resources
    inflateEnd(&strm);
    gzclose(gzfile);
    free(dest);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}