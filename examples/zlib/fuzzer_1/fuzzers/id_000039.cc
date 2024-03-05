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
//<ID> 639
//<Prompt> ["gzopen","gzputc","inflateInit2_","inflateGetDictionary","inflatePrime","inflateCodesUsed","gzclose","inflateEnd"]
/*<Combination>: [gzFile gzopen(const char * , const char * ),
    int gzputc(gzFile file, int c),
    int inflateInit2_(z_streamp strm, int windowBits, const char * version, int stream_size),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int inflatePrime(z_streamp strm, int bits, int value),
    unsigned long inflateCodesUsed(z_streamp ),
    int gzclose(gzFile file),
    int inflateEnd(z_streamp strm)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["gzopen","gzclose","inflateInit2_","inflateGetDictionary","inflatePrime","inflateCodesUsed","inflateEnd"],"critical_calls":["gzopen","gzclose","inflateInit2_","inflateGetDictionary","inflatePrime","inflateCodesUsed","inflateEnd"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
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





    gzFile file = gzopen("input_file", fuzz_str_1);
    if (file == NULL) {
        assert_file_name_closed("input_file");
	return -1;
    }

    // Read the input data
    int ret;
    while ((ret = gzgetc(file)) != -1) {
        // Process the input data
    }

    // Close the input file
    gzclose(file);

    // Open the output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return -1;
    }

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Initialize the inflater
    if (inflateInit2_(&strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return -1;
    }

    // Get the dictionary
    Bytef dictionary[1024];
    uInt dictLength = sizeof(dictionary);
    if (inflateGetDictionary(&strm, dictionary, &dictLength) != Z_OK) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return -1;
    }

    // Prime the inflater
    if (inflatePrime(&strm, 8, fuzz_int32_t_2) != Z_OK) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return -1;
    }

    // Get the number of codes used by the inflater
    unsigned long codesUsed = inflateCodesUsed(&strm);

    // Close the output file
    assert_file_closed(&out_file);;

    // End the inflater
    if (inflateEnd(&strm) != Z_OK) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return -1;
    }

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return 0;
}