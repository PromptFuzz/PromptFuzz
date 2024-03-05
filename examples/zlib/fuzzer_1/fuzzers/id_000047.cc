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
//<ID> 943
//<Prompt> ["gzopen","deflateGetDictionary","gzprintf","inflateUndermine","gzgets","gzgetc_","compressBound","compress","gzclose"]
/*<Combination>: [gzFile gzopen(const char * , const char * ),
    int deflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int gzprintf(gzFile file, const char * format),
    int inflateUndermine(z_streamp , int ),
    char *gzgets(gzFile file, char * buf, int len),
    int gzgetc_(gzFile file),
    uLong compressBound(uLong sourceLen),
    int compress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int gzclose(gzFile file)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["gzopen","deflateGetDictionary","gzprintf","inflateUndermine","gzgets","gzgetc_","compressBound","compress","gzclose"],"critical_calls":["gzopen","deflateGetDictionary","gzprintf","inflateUndermine","gzgets","gzgetc_","compressBound","compress","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
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
	return 0;
    }
    
    // Get dictionary
    z_stream strm;
    Bytef dictionary[1024];
    uInt dictLength;
    int result = deflateGetDictionary(&strm, dictionary, &dictLength);
    
    // Write formatted string to file
    gzprintf(file, "Formatted string: %s\n", "Hello World!");
    
    // Undermine the inflate process
    result = inflateUndermine(&strm, fuzz_int32_t_2);
    
    // Read a line from file
    char buf[1024];
    gzgets(file, buf, sizeof(buf));
    
    // Get a character from file
    int c = gzgetc_(file);
    
    // Compress the input data
    uLongf destLen = compressBound(size);
    Bytef* dest = new Bytef[destLen];
    result = compress(dest, &destLen, data, size);
    
    // Write the compressed data to output file
    FILE* output = fopen("output_file", "wb");
    fwrite(dest, 1, destLen, output);
    assert_file_closed(&output);;
    
    // Clean up
    gzclose(file);
    delete[] dest;
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&output);
	return 0;
}