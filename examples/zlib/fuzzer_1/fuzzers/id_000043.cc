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
//<ID> 813
//<Prompt> ["gzfread","inflateSetDictionary","inflateResetKeep","gzopen","inflateGetHeader","inflateReset2","inflateBackInit_","compress"]
/*<Combination>: [z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int inflateResetKeep(z_streamp ),
    gzFile gzopen(const char * , const char * ),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    int inflateReset2(z_streamp strm, int windowBits),
    int inflateBackInit_(z_streamp strm, int windowBits, unsigned char * window, const char * version, int stream_size),
    int compress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen)
*/
//<score> 5, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"inflateBackInit_":[[30,9,30,26,0,0,4,0]]},"library_calls":["gzopen","gzopen","inflateSetDictionary","inflateResetKeep","inflateGetHeader","inflateReset2","inflateBackInit_","compress","gzfread","gzclose","gzclose"],"critical_calls":["gzopen","gzopen","inflateSetDictionary","inflateResetKeep","inflateGetHeader","inflateReset2","inflateBackInit_","compress","gzfread","gzclose","gzclose"],"visited":1}
/*Here is the C++ code that achieves the event using the zlib library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Step 4: Create FILE * for input and output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors for input and output
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 6: Create file name strings
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    gzFile input_gzfile, output_gzfile;
    z_stream strm;
    gz_header head;

    // Step 2: Call zlib APIs
    input_gzfile = gzopen(input_file, fuzz_str_1);
    output_gzfile = gzopen(output_file, fuzz_str_2);

    inflateSetDictionary(&strm, nullptr, 0);
    inflateResetKeep(&strm);
    inflateGetHeader(&strm, &head);
    inflateReset2(&strm, fuzz_int32_t_3);
    inflateBackInit_(&strm, 15, nullptr, nullptr, 0);

    Bytef source[1000];
    uLongf destLen = sizeof(source);
    compress(source, &destLen, data, size);

    gzfread(source, sizeof(Bytef), sizeof(source), input_gzfile);

    // Step 7: Release allocated resources
    gzclose(input_gzfile);
    gzclose(output_gzfile);
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