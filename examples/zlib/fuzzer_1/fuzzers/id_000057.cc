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
//<ID> 1008
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
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzclearerr","gzungetc","gzoffset","gzclose","compress2","gzclose","gzclose"],"critical_calls":["gzdopen","gzclearerr","gzungetc","gzoffset","compress2","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create a FILE pointer for reading the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the input file name
    const char* input_file = "input_file";
	int fuzz_fd_0 = fuzz_fileno(in_file);


    // Step 1: Open and read the gzipped file
    gzFile gz = gzdopen(fuzz_fd_0, fuzz_str_1);
    if (gz == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }

    // Step 2: Clear the error flags and indicators of the gzipped file
    gzclearerr(gz);

    // Step 3: Unget a character from the gzipped file
    int c = gzgetc(gz);
    gzungetc(c, gz);

    // Step 7: Get the offset of the gzipped file
    off_t offset = gzoffset(gz);

    // Step 9: Create a FILE pointer for writing the output data
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }

    // Step 5: Specify the output file name
    const char* output_file = "output_file";

    // Step 8: Compress the data from the input file and write to the output file
    Bytef buffer[1024];
    uLongf buffer_len = sizeof(buffer);
    int compress_result = compress2(buffer, &buffer_len, (const Bytef*)data, size, fuzz_int32_t_2);
    if (compress_result != Z_OK) {
        gzclose(gz);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }

    fwrite(buffer, 1, buffer_len, out_file);

    // Step 10: Close the gzipped file
    gzclose(gz);

    // Step 11: Close the input and output files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}