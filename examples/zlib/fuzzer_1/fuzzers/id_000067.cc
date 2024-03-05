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
//<ID> 1096
//<Prompt> ["gzopen","gzopen64","gzeof","gzread","gzbuffer","gztell64","inflateSync","gzflush","deflateSetDictionary","deflateTune","inflateGetHeader","gzputc","gzdirect","adler32_combine","gzclose"]
/*<Combination>: [gzFile gzopen(const char * , const char * ),
    gzFile gzopen64(const char * , const char * ),
    int gzeof(gzFile file),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int gzbuffer(gzFile file, unsigned int size),
    off64_t gztell64(gzFile ),
    int inflateSync(z_streamp strm),
    int gzflush(gzFile file, int flush),
    int deflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    int gzputc(gzFile file, int c),
    int gzdirect(gzFile file),
    uLong adler32_combine(uLong , uLong , off_t ),
    int gzclose(gzFile file)
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["gzopen","gzread","gzeof","gzopen","gzflush","gztell64","gzclose","gzclose"],"critical_calls":["gzopen","gzread","gzeof","gzopen","gzflush","gztell64","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
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





    // Step 4: Create a FILE * variable for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Set the input file name
    const char *in_file_name = "input_file";

    // Step 1: Open the input file as a gzFile for reading
    gzFile input_gzfile = gzopen(in_file_name, fuzz_str_1);

    // Event: Read from the gzFile, check if it is at the end of file
    int read_size;
    char buffer[1024];
    while ((read_size = gzread(input_gzfile, buffer, sizeof(buffer))) > 0) {
        // Process the read data if needed
    }
    int is_eof = gzeof(input_gzfile);

    // Step 2: Create a gzFile for writing compressed data
    gzFile output_gzfile = gzopen("output_file", fuzz_str_2);

    // Event: Flush the output gzFile
    gzflush(output_gzfile, fuzz_int32_t_3);

    // Step 3: Get the position in the input gzFile
    off64_t input_gzfile_position = gztell64(input_gzfile);

    // Step 7: Release allocated resources
    gzclose(input_gzfile);
    gzclose(output_gzfile);
    assert_file_closed(&in_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}