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
//<ID> 85
//<Prompt> ["inflateBackEnd","inflateUndermine","inflateGetDictionary","gzwrite","deflateSetDictionary","uncompress","inflateReset2","inflateSetDictionary"]
/*<Combination>: [int inflateBackEnd(z_streamp strm),
    int inflateUndermine(z_streamp , int ),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int gzwrite(gzFile file, voidpc buf, unsigned int len),
    int deflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int inflateReset2(z_streamp strm, int windowBits),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength)
*/
//<score> 17.5, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"gzwrite":[[241,9,241,21,0,0,4,0]],"inflateSetDictionary":[[1305,9,1305,32,0,0,4,0]],"inflateUndermine":[[1486,9,1486,32,0,0,4,0]],"inflateGetDictionary":[[1283,9,1283,32,0,0,4,0]]},"library_calls":["inflateBackEnd","inflateUndermine","inflateGetDictionary","gzwrite","deflateSetDictionary","uncompress","inflateReset2","inflateSetDictionary","inflateEnd"],"critical_calls":["inflateBackEnd","inflateUndermine","inflateGetDictionary","gzwrite","deflateSetDictionary","uncompress","inflateReset2","inflateSetDictionary","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Open a file to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0; // Error handling
    }

    // Step 6: Specify the file name for input file
    const char *input_file = "input_file";

    // Step 7: Release the resources of the input file
    assert_file_closed(&in_file);;

    // Step 4: Open a file to write output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0; // Error handling
    }

    // Step 6: Specify the file name for output file
    const char *output_file = "output_file";

    // Step 7: Release the resources of the output file
    assert_file_closed(&out_file);;

    // Step 4: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 4: Get the file descriptor for writing
    int out_fd = fuzz_fileno(out_file);

    // Step 7: Release the resources of the file descriptors
    close(in_fd);
    close(out_fd);

    // Step 1: Use zlib APIs to achieve the desired event
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    // Step 2: Call zlib APIs
    int result = inflateBackEnd(&strm);
    result = inflateUndermine(&strm, fuzz_int32_t_1);
    result = inflateGetDictionary(&strm, NULL, NULL);
    result = gzwrite(NULL, NULL, 0);
    result = deflateSetDictionary(&strm, NULL, 0);
    Bytef dest[100];
    uLongf destLen = sizeof(dest);
    result = uncompress(dest, &destLen, NULL, 0);
    result = inflateReset2(&strm, fuzz_int32_t_2);
    result = inflateSetDictionary(&strm, NULL, 0);

    // Step 7: Release the resources of the z_stream variable
    inflateEnd(&strm);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}