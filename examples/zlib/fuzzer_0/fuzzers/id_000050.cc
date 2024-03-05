#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 958
//<Prompt> ["gzputs","deflateSetDictionary","zlibCompileFlags","gzread"]
/*<Combination>: [int gzputs(gzFile file, const char * s),
    int deflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    uLong zlibCompileFlags(),
    int gzread(gzFile file, voidp buf, unsigned int len)
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["gzdopen","gzgets","gzclose","deflateSetDictionary","zlibCompileFlags","gzdopen","gzread","gzclose"],"critical_calls":["gzdopen","gzgets","gzclose","deflateSetDictionary","zlibCompileFlags","gzdopen","gzread","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create FILE variables to read input data and write output data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");

    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 2: Use zlib APIs to achieve the event
    // Call gzputs to write input data to output file
    gzFile input_file = gzdopen(in_fd, "rb");
    char buf[1024];
    while (gzgets(input_file, buf, sizeof(buf)) != NULL) {
        fputs(buf, out_file);
    }
    gzclose(input_file);

    // Call deflateSetDictionary to set a dictionary for compression
    z_stream strm;
    Bytef dictionary[10] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A};
    deflateSetDictionary(&strm, dictionary, sizeof(dictionary));

    // Call zlibCompileFlags to get the compile flags of zlib
    uLong compile_flags = zlibCompileFlags();

    // Call gzread to read from input file and write to output file
    gzFile output_file = gzdopen(out_fd, "wb");
    char read_buf[1024];
    while (gzread(output_file, read_buf, sizeof(read_buf)) > 0) {
        fwrite(read_buf, sizeof(char), sizeof(read_buf), out_file);
    }
    gzclose(output_file);

    // Step 7: Close files and release resources 
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}