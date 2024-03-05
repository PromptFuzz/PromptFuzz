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
//<ID> 1002
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
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzclearerr","gzungetc","gzprintf","gzoffset","gzoffset64","uncompress","inflateGetHeader","compress2","deflatePrime","gzclose"],"critical_calls":["gzdopen","gzclearerr","gzungetc","gzprintf","gzoffset","gzoffset64","uncompress","inflateGetHeader","compress2","deflatePrime","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE * variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 4: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");

    if (in_file == nullptr || out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Use file names directly
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Open the gzFile
    gzFile gz_file = gzdopen(in_fd, fuzz_str_1);

    // Use gzclearerr to clear error flags
    gzclearerr(gz_file);

    // Step 2: Use gzungetc to put back a character
    gzungetc('A', gz_file);

    // Step 3: Use gzprintf to write a formatted string to the gzFile
    gzprintf(gz_file, "Hello, World!\n");

    // Step 7: Use gzoffset to get the offset in the gzFile
    off_t offset = gzoffset(gz_file);

    // Step 7: Use gzoffset64 to get the offset in the gzFile
    off64_t offset64 = gzoffset64(gz_file);

    // Step 1: Use uncompress to decompress the data
    // Create a buffer for the decompressed data
    uLongf dest_len = size * 10;
    Bytef *dest = (Bytef *)malloc(dest_len);
    memset(dest, 0, dest_len);

    // Use uncompress to decompress the data
    uncompress(dest, &dest_len, data, size);

    // Step 2: Inflate the zlib header
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    inflateGetHeader(nullptr, &header);

    // Step 2: Use compress2 to compress the data
    uLongf compressed_len = dest_len;
    Bytef *compressed_data = (Bytef *)malloc(compressed_len);
    memset(compressed_data, 0, compressed_len);
    compress2(compressed_data, &compressed_len, dest, dest_len, fuzz_int32_t_2);

    // Step 2: Use deflatePrime to add bits to the output buffer
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    deflatePrime(&strm, fuzz_int32_t_3, fuzz_int32_t_4);

    // Step 1: Close the gzFile
    gzclose(gz_file);

    // Step 7: Release allocated resources
    free(dest);
    free(compressed_data);
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