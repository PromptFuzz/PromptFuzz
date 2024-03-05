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
//<ID> 503
//<Prompt> ["gzputs","gzeof","inflateBack","deflatePrime","uncompress","gzrewind"]
/*<Combination>: [int gzputs(gzFile file, const char * s),
    int gzeof(gzFile file),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    int deflatePrime(z_streamp strm, int bits, int value),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int gzrewind(gzFile file)
*/
//<score> 4, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gzread":[[355,36,355,61,0,0,4,0]]},"library_calls":["gzopen","gzgets","gzeof","gzrewind","gzclose","gzeof","gzread","gzclose"],"critical_calls":["gzopen","gzgets","gzeof","gzrewind","gzeof","gzread","gzclose"],"visited":3}
/*Here is the complete C++ program that achieves the event using the zlib library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<5) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    gzFile file = gzopen("input_file", fuzz_str_1);
    if (!file) {
        assert_file_name_closed("input_file");
	return 0;
    }

    // Read data from the input file
    int eof = 0;
    while (!eof) {
        char buffer[128];
        if (gzgets(file, buffer, sizeof(buffer)) == NULL) {
            if (gzeof(file)) {
                eof = 1;
            } else {
                break;
            }
        } else {
            // Do something with the read data
        }
    }

    gzrewind(file);

    // Write data to the output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(file);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return 0;
    }

    char out_buffer[128];
    while (!gzeof(file)) {
        int len = gzread(file, out_buffer, sizeof(out_buffer));
        if (len <= 0) {
            break;
        }
        fwrite(out_buffer, 1, len, out_file);
    }

    assert_file_closed(&out_file);;
    gzclose(file);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return 0;
}