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
//<ID> 9
//<Prompt> ["gzseek","uncompress2","gzoffset64","gzopen64","inflateReset2","zError"]
/*<Combination>: [off_t gzseek(gzFile , off_t , int ),
    int uncompress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong * sourceLen),
    off64_t gzoffset64(gzFile ),
    gzFile gzopen64(const char * , const char * ),
    int inflateReset2(z_streamp strm, int windowBits),
    const char *zError(int )
*/
//<score> 4.5, nr_unique_branch: 3
//<Quality> {"density":3,"unique_branches":{"gzoffset64":[[451,35,451,58,0,0,4,1],[458,9,458,31,0,0,4,1]],"gzseek64":[[404,9,404,15,0,0,4,1]]},"library_calls":["gzopen64","gzseek64","gzclose","uncompress2","gzopen64","gzoffset64","gzclose","inflateReset2","inflateEnd","zError"],"critical_calls":["gzopen64","gzseek64","gzclose","uncompress2","gzopen64","gzoffset64","gzclose","inflateReset2","inflateEnd","zError"],"visited":1}
/*Here is the C++ code that achieves the event using the zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create FILE * variables for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Use the file name string directly
    const char *input_file = "input_file";

    // Step 7: Close the input file and release resources
    assert_file_closed(&in_file);;

    // Step 4: Create FILE * variables for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Use the file name string directly
    const char *output_file = "output_file";

    // Step 7: Close the output file and release resources
    assert_file_closed(&out_file);;

    // Step 1: Seek to a specific position in the gzFile
    gzFile gz_input_file = gzopen64(input_file, fuzz_str_1);
    if (gz_input_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    off64_t seek_pos = gzseek64(gz_input_file, 0, fuzz_int32_t_2);

    // Step 7: Close the gzFile and release resources
    gzclose(gz_input_file);

    // Step 1: Uncompress the input data using uncompress2
    Bytef *dest = new Bytef[size];
    uLongf destLen = size;
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, data, &sourceLen);

    // Step 7: Release the allocated memory
    delete[] dest;

    // Step 1: Get the offset of the gzFile
    gzFile gz_output_file = gzopen64(output_file, fuzz_str_3);
    if (gz_output_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    off64_t offset = gzoffset64(gz_output_file);

    // Step 7: Close the gzFile and release resources
    gzclose(gz_output_file);

    // Step 1: Reset the inflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int windowBits = 15;
    int reset_result = inflateReset2(&strm, fuzz_int32_t_4);

    // Step 7: Release the inflate stream resources
    inflateEnd(&strm);

    // Step 1: Get the zlib error message
    const char *error_message = zError(-1);

    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}