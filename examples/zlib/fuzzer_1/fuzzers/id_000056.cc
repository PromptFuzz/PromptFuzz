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
//<ID> 1003
//<Prompt> ["gzdopen","gzclearerr","gzungetc","gzprintf","inflateGetHeader","uncompress","gzoffset","gzoffset64","compress2","deflatePrime","gzclose"]
/*<Combination>: [gzFile gzdopen(int fd, const char * mode),
    void gzclearerr(gzFile file),
    int gzungetc(int c, gzFile file),
    int gzprintf(gzFile file, const char * format),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    off_t gzoffset(gzFile ),
    off64_t gzoffset64(gzFile ),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    int deflatePrime(z_streamp strm, int bits, int value),
    int gzclose(gzFile file)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzclearerr","gzungetc","gzprintf","inflateGetHeader","compressBound","uncompress","gzoffset","gzoffset64","compress2","deflatePrime","gzclose"],"critical_calls":["gzdopen","gzclearerr","gzungetc","gzprintf","inflateGetHeader","compressBound","uncompress","gzoffset","gzoffset64","compress2","deflatePrime","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    gzFile file = gzdopen(in_fd, fuzz_str_1);
    gzclearerr(file);
    gzungetc('A', file);

    gzprintf(file, "Hello, %s!", "world");

    z_stream strm;
    gz_header head;
    inflateGetHeader(&strm, &head);

    uLongf destLen = compressBound(size);
    Bytef *dest = new Bytef[destLen];
    uncompress(dest, &destLen, data, size);

    off_t offset = gzoffset(file);
    off64_t offset64 = gzoffset64(file);

    int level = 6;
    compress2(dest, &destLen, data, size, fuzz_int32_t_2);

    int bits = 5;
    int value = 10;
    deflatePrime(&strm, fuzz_int32_t_3, fuzz_int32_t_4);

    gzclose(file);

    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    delete[] dest;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}