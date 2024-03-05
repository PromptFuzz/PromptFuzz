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
//<ID> 303
//<Prompt> ["deflateGetDictionary","gzsetparams","compress2","deflateCopy","inflateGetDictionary","inflateBackEnd","inflateSyncPoint"]
/*<Combination>: [int deflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int gzsetparams(gzFile file, int level, int strategy),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    int deflateCopy(z_streamp dest, z_streamp source),
    int inflateGetDictionary(z_streamp strm, Bytef * dictionary, uInt * dictLength),
    int inflateBackEnd(z_streamp strm),
    int inflateSyncPoint(z_streamp )
*/
//<score> 6, nr_unique_branch: 5
//<Quality> {"density":3,"unique_branches":{"deflateGetDictionary":[[625,9,625,32,0,0,4,1],[629,9,629,24,0,0,4,1],[631,9,631,29,0,0,4,0],[631,33,631,36,0,0,4,1],[633,9,633,29,0,0,4,0]]},"library_calls":["deflateInit_","deflateGetDictionary","gzdopen","gzsetparams","compressBound","compress2","deflateEnd","gzclose"],"critical_calls":["deflateInit_","deflateGetDictionary","gzdopen","gzsetparams","compressBound","compress2","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
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





    z_stream strm;
    
    // Step 4: Create input file stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    
    // Check if the input file is valid
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptor for input file
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Set input file name
    const char *in_file_name = "input_file";
    
    // Initialize zlib stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize zlib for deflate
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 1: Use deflateGetDictionary to get the dictionary from zlib's output data
    Bytef dictionary[256];
    uInt dictLength = sizeof(dictionary);
    deflateGetDictionary(&strm, dictionary, &dictLength);
    
    // Step 2: Use gzsetparams to set the compression level and strategy
    gzFile gzfile = gzdopen(in_fd, fuzz_str_1);
    gzsetparams(gzfile, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Step 3: Use compress2 to compress the input data with the specified compression level
    uLongf destLen = compressBound(size);
    Bytef* compressedData = (Bytef*)malloc(sizeof(Bytef) * destLen);
    compress2(compressedData, &destLen, (const Bytef *)data, size, fuzz_int32_t_4);
    
    // Step 7: Cleanup
    deflateEnd(&strm);
    gzclose(gzfile);
    assert_file_closed(&in_file);;
    free(compressedData);
    
    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}