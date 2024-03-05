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
//<ID> 1614
//<Prompt> ["gzgetc","gzclose_r","gzopen","gzgets","gzeof","gzrewind","gzsetparams","gzread","gzclose"]
/*<Combination>: [int gzgetc(gzFile file),
    int gzclose_r(gzFile file),
    gzFile gzopen(const char * , const char * ),
    char *gzgets(gzFile file, char * buf, int len),
    int gzeof(gzFile file),
    int gzrewind(gzFile file),
    int gzsetparams(gzFile file, int level, int strategy),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int gzclose(gzFile file)
*/
//<score> 22, nr_unique_branch: 8
//<Quality> {"density":11,"unique_branches":{"deflate":[[1210,24,1210,40,0,0,4,1],[133,31,133,38,10,0,4,0],[133,31,133,38,11,0,4,0]],"deflate_slow":[[1599,8,1599,31,27,0,4,0],[1586,24,1586,44,44,0,4,1]],"gzsetparams":[[583,9,583,20,0,0,4,0],[585,13,585,27,0,0,4,0],[585,31,585,60,0,0,4,1]]},"library_calls":["gzopen","gzopen","gzclose","gzread","gzwrite","gzeof","gzrewind","gzgets","gzsetparams","gzclose_r","gzclose"],"critical_calls":["gzopen","gzopen","gzread","gzwrite","gzeof","gzrewind","gzgets","gzsetparams","gzclose_r","gzclose"],"visited":1}
/*Here is a C++ program that achieves the event using the zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a FILE pointer to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE pointer to write the output data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Open the input file using gzopen
    gzFile in_gz = gzopen("input_file", "rb");
    if (in_gz == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Open the output file using gzopen
    gzFile out_gz = gzopen("output_file", fuzz_str_1);
    if (out_gz == nullptr) {
        gzclose(in_gz);
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
    
    // Read from the input file using gzread
    char buf[1024];
    int len;
    while ((len = gzread(in_gz, buf, sizeof(buf))) > 0) {
        // Write to the output file using gzwrite
        gzwrite(out_gz, buf, len);
    }
    
    // Check if the end of file is reached using gzeof
    if (gzeof(in_gz)) {
        printf("End of input file reached.\n");
    }
    
    // Rewind the input file using gzrewind
    gzrewind(in_gz);
    
    // Read a line from the input file using gzgets
    char line[256];
    gzgets(in_gz, line, sizeof(line));
    
    // Set compression parameters using gzsetparams
    gzsetparams(out_gz, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Close the input and output files using gzclose_r and gzclose
    gzclose_r(in_gz);
    gzclose(out_gz);
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