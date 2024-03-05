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
//<ID> 618
//<Prompt> ["deflateResetKeep","gztell64","gzputc","compress2","deflateCopy","inflateGetDictionary","inflateBack"]
/*<Combination>: [int deflateResetKeep(z_streamp ),
    off64_t gztell64(gzFile ),
    int gzputc(gzFile file, int c),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    int deflateCopy(z_streamp dest, z_streamp source),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc)
*/
//<score> 3, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"gzputc":[[294,9,294,21,0,0,4,0]]},"library_calls":["deflateResetKeep","gztell64","gzputc","compress2","deflateCopy","inflateGetDictionary","inflateBack"],"critical_calls":["deflateResetKeep","gztell64","gzputc","compress2","deflateCopy","inflateGetDictionary","inflateBack"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Step 1: Define variables
    z_stream strm;
    gzFile file;
    FILE *in_file;
    FILE *out_file;
    int fd;

    // Step 2: Initialize variables
    deflateResetKeep(&strm); // API 1
    gztell64(file); // API 2
    gzputc(file, 'c'); // API 3
    Bytef dest[100];
    uLongf destLen = sizeof(dest);
    const Bytef *source = data;
    uLong sourceLen = size;
    int level = Z_DEFAULT_COMPRESSION;
    compress2(dest, &destLen, source, sourceLen, fuzz_int32_t_1); // API 4
    z_streamp destStrm, sourceStrm;
    deflateCopy(destStrm, sourceStrm); // API 5
    Bytef dictionary[100];
    uInt dictLength = sizeof(dictionary);
    inflateGetDictionary(&strm, dictionary, &dictLength); // API 6
    in_func in;
    void *in_desc;
    out_func out;
    void *out_desc;
    inflateBack(&strm, in, in_desc, out, out_desc); // API 7

    // Step 3: Consume input data
    in_file = fmemopen((void*)data, size, "rb"); // Step 4
    fd = fuzz_fileno(in_file); // Step 5
    close(fd); // Consume the file descriptor

    // Step 4: Create output file
    out_file = fopen("output_file", "wb"); // Step 6
    fd = fuzz_fileno(out_file); // Step 5
    close(fd); // Consume the file descriptor

    // Step 5: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	return 0;
}