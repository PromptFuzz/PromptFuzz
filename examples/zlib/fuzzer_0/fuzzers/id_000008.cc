#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 178
//<Prompt> ["crc32","inflateResetKeep","deflateTune","inflateCopy","deflateResetKeep","crc32_combine_gen","gzbuffer","gzclearerr","gzseek"]
/*<Combination>: [uLong crc32(uLong crc, const Bytef * buf, uInt len),
    int inflateResetKeep(z_streamp ),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int inflateCopy(z_streamp dest, z_streamp source),
    int deflateResetKeep(z_streamp ),
    uLong crc32_combine_gen(off_t ),
    int gzbuffer(gzFile file, unsigned int size),
    void gzclearerr(gzFile file),
    off_t gzseek(gzFile , off_t , int )
*/
//<score> 1.5, nr_unique_branch: 2
//<Quality> {"density":1,"unique_branches":{"gzclearerr":[[509,9,509,21,0,0,4,0]],"gzseek64":[[345,9,345,21,0,0,4,0]]},"library_calls":["crc32","inflateResetKeep","deflateTune","inflateCopy","deflateResetKeep","crc32_combine_gen","gzbuffer","gzclearerr","gzseek"],"critical_calls":["crc32","inflateResetKeep","deflateTune","inflateCopy","deflateResetKeep","crc32_combine_gen","gzbuffer","gzclearerr","gzseek"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        std::cerr << "Failed to open input file." << std::endl;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify the input file name
    const char *input_filename = "input_file";

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 4: Create a FILE * variable to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        std::cerr << "Failed to open output file." << std::endl;
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Specify the output file name
    const char *output_filename = "output_file";

    // Step 5: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Use zlib APIs to manipulate the data
    uLong crc = crc32(0L, Z_NULL, 0);
    inflateResetKeep(nullptr);
    deflateTune(nullptr, 256, 8, 120, 4);
    inflateCopy(nullptr, nullptr);
    deflateResetKeep(nullptr);
    uLong crc_combine = crc32_combine_gen(0);
    gzbuffer(nullptr, 1024);
    gzclearerr(nullptr);
    gzseek(nullptr, 0, SEEK_SET);

    // Step 7: Release allocated resources
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