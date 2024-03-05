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
//<ID> 208
//<Prompt> ["gzclose_w","inflateSetDictionary","deflateEnd","inflatePrime","adler32_combine64","gzclearerr","gzdirect"]
/*<Combination>: [int gzclose_w(gzFile file),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int deflateEnd(z_streamp strm),
    int inflatePrime(z_streamp strm, int bits, int value),
    uLong adler32_combine64(uLong , uLong , off64_t ),
    void gzclearerr(gzFile file),
    int gzdirect(gzFile file)
*/
//<score> 9, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"gzdirect":[[569,35,569,53,0,0,4,1]],"gzclose_w":[[605,9,605,32,0,0,4,0]]},"library_calls":["gzopen","gzread","gzclose_w","inflateSetDictionary","deflateEnd","inflatePrime","adler32_combine64","gzclearerr","gzdirect","gzclose"],"critical_calls":["gzopen","gzread","gzclose_w","inflateSetDictionary","deflateEnd","inflatePrime","adler32_combine64","gzclearerr","gzdirect","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<25) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
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
    
    // Read data from input file
    uint8_t buffer[1024];
    int bytesRead;
    while ((bytesRead = gzread(file, buffer, sizeof(buffer))) > 0) {
        // Process data using zlib APIs
        // Step 1
        gzclose_w(file);
        z_stream strm;
        inflateSetDictionary(&strm, buffer, bytesRead);
        deflateEnd(&strm);
        inflatePrime(&strm, 8, fuzz_int32_t_2);
        adler32_combine64(fuzz_uint64_t_3, fuzz_uint64_t_4, 0);
        gzclearerr(file);
        gzdirect(file);
        
        // Step 2 - Add more calls to zlib APIs if desired
        
        // Step 3 - Consume input data
        
        // Step 4 - Create FILE variables for input/output
        
        // Step 5 - Obtain file descriptors
        
        // Step 6 - Use file names
        
        // Step 7 - Release resources
    }
    
    gzclose(file);
    assert_file_name_closed("input_file");
	return 0;
}