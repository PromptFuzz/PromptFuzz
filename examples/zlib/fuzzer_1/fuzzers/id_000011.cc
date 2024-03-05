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
//<ID> 191
//<Prompt> ["gzbuffer","gztell64","inflateSync","gzflush","deflateSetDictionary","inflateGetHeader","gzputc","adler32_combine"]
/*<Combination>: [int gzbuffer(gzFile file, unsigned int size),
    off64_t gztell64(gzFile ),
    int inflateSync(z_streamp strm),
    int gzflush(gzFile file, int flush),
    int deflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    int gzputc(gzFile file, int c),
    uLong adler32_combine(uLong , uLong , off_t )
*/
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"gz_comp":[[92,13,92,32,0,0,4,1]],"gzputc":[[313,13,313,32,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzbuffer","gztell64","inflateSync","gzflush","deflateSetDictionary","inflateGetHeader","gzputc","adler32_combine","adler32","adler32","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","gzbuffer","gztell64","inflateSync","gzflush","deflateSetDictionary","inflateGetHeader","gzputc","adler32_combine","adler32","adler32","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<1061) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeCharArray(unsigned char, fuzz_char_array_5, fuzz_char_array_size_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	
		const Bytef * fuzzer_var_deflateSetDictionary_6_1 = fuzz_char_array_5;
		uInt fuzzer_size_deflateSetDictionary_6_1 = static_cast<uInt>(fuzz_char_array_size_5);//fuzzer shim end}




    // Step 4: Create input and output file streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    
    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 2: Use zlib APIs to achieve the desired event
    gzFile input_gz = gzdopen(in_fd, fuzz_str_1);
    gzFile output_gz = gzdopen(out_fd, fuzz_str_2);
    
    gzbuffer(input_gz, fuzz_uint32_t_3); // Call gzbuffer to set input buffer size
    
    off64_t pos = gztell64(input_gz); // Call gztell64 to get current position in input gzFile
    
    z_stream zstrm;
    zstrm.zalloc = Z_NULL;
    zstrm.zfree = Z_NULL;
    zstrm.next_in = (Bytef *)data;
    zstrm.avail_in = (uInt)size;
    inflateSync(&zstrm); // Call inflateSync to find a valid zlib stream within input
    
    gzflush(output_gz, fuzz_int32_t_4); // Call gzflush to flush remaining data in output gzFile
    
    Bytef dictionary[1024] = {0}; // Define a dictionary buffer
    uInt dictLength = 1024;
    deflateSetDictionary(&zstrm, fuzzer_var_deflateSetDictionary_6_1, fuzzer_size_deflateSetDictionary_6_1); // Call deflateSetDictionary to set compression dictionary
    
    gz_header gzhdr;
    inflateGetHeader(&zstrm, &gzhdr); // Call inflateGetHeader to get gzip header information
    
    gzputc(output_gz, 'A'); // Call gzputc to write a character to the output gzFile
    
    uLong combined_adler = adler32_combine(adler32(fuzz_uint64_t_6, Z_NULL, 0), adler32(fuzz_uint64_t_7, Z_NULL, 0), 0); // Call adler32_combine to combine Adler-32 checksums
    
    // Step 7: Release allocated resources
    gzclose(input_gz);
    gzclose(output_gz);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}