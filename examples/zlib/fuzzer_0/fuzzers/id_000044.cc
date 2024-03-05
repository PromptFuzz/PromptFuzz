#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 899
//<Prompt> ["inflateMark","gzrewind","gzoffset","deflatePrime","inflateGetDictionary","compress2","gztell","deflateBound"]
/*<Combination>: [long inflateMark(z_streamp strm),
    int gzrewind(gzFile file),
    off_t gzoffset(gzFile ),
    int deflatePrime(z_streamp strm, int bits, int value),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    off_t gztell(gzFile ),
    uLong deflateBound(z_streamp strm, uLong sourceLen)
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"gzoffset64":[[448,9,448,21,0,0,4,0]]},"library_calls":["inflateMark","gzrewind","gzoffset","deflatePrime","inflateGetDictionary","compressBound","compress2","gztell","deflateBound"],"critical_calls":["inflateMark","gzrewind","gzoffset","deflatePrime","inflateGetDictionary","compressBound","compress2","gztell","deflateBound"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 1: Declare variables
    z_stream stream;
    gzFile file;
    FILE *in_file;
    FILE *out_file;

    // Step 2: Initialize variables
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    // Step 3: Set input
    in_file = fmemopen((void *)data, size, "rb");
    int in_fd = fuzz_fileno(in_file);
    stream.avail_in = size;
    stream.next_in = (Bytef *)malloc(size);
    if (stream.next_in == NULL) {
        assert_file_closed(&in_file);;
        assert_fd_closed(in_fd);
	return 0;
    }
    fread(stream.next_in, 1, size, in_file);

    // Step 4: Set output
    out_file = fopen("output_file", "wb");
    int out_fd = fuzz_fileno(out_file);

    // Step 5: Perform operations
    long mark = inflateMark(&stream);
    gzrewind(file);
    off_t offset = gzoffset(file);
    int bits = 8;
    int value = 1;
    deflatePrime(&stream, bits, value);
    Bytef dictionary[1024];
    uInt dictLength = 1024;
    inflateGetDictionary(&stream, dictionary, &dictLength);
    uLongf destLen = compressBound(stream.avail_in);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (dest == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(stream.next_in);
        assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    int level = 6;
    compress2(dest, &destLen, stream.next_in, stream.avail_in, level);
    off_t tell = gztell(file);
    uLong bound = deflateBound(&stream, stream.avail_in);

    // Step 6: Release resources
    free(stream.next_in);
    free(dest);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}