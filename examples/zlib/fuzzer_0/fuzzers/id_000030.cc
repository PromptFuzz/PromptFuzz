#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 502
//<Prompt> ["gzputs","gzeof","inflateBack","deflatePrime","uncompress","gzrewind"]
/*<Combination>: [int gzputs(gzFile file, const char * s),
    int gzeof(gzFile file),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    int deflatePrime(z_streamp strm, int bits, int value),
    int uncompress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int gzrewind(gzFile file)
*/
//<score> 3, nr_unique_branch: 1
//<Quality> {"density":3,"unique_branches":{"gzwrite":[[246,9,246,32,0,0,4,0]]},"library_calls":["gzopen","gzwrite","gzclose","gzopen","gzgets","gzclose"],"critical_calls":["gzopen","gzwrite","gzclose","gzopen","gzgets","gzclose"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    gzFile file = gzopen("input_file", "rb");
    gzwrite(file, data, size);
    gzclose(file);
    
    file = gzopen("input_file", "rb");
    char buffer[1024];
    while (gzgets(file, buffer, sizeof(buffer))) {
        std::cout << buffer;
    }
    gzclose(file);
    
    assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}