#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1072
//<Prompt> ["compress2","gzputs","gzopen","gzread","gzclose","inflateUndermine","deflateEnd"]
/*<Combination>: [int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    int gzputs(gzFile file, const char * s),
    gzFile gzopen(const char * , const char * ),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int gzclose(gzFile file),
    int inflateUndermine(z_streamp , int ),
    int deflateEnd(z_streamp strm)
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzopen","gzwrite","gzclose","gzopen","gzread","gzread","gzclose"],"critical_calls":["gzopen","gzwrite","gzclose","gzopen","gzread","gzread","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 4: Create the input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get the input file descriptor
    int in_fd = fuzz_fileno(in_file);
    
    // Step 6: Define the output file name
    const char *out_file_name = "output_file";
    
    // Step 1: Compress the input file and write it to the output file
    gzFile out_file = gzopen(out_file_name, "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    unsigned char buf[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buf, 1, sizeof(buf), in_file)) > 0) {
        gzwrite(out_file, buf, bytesRead);
    }
    
    gzclose(out_file);
    
    // Step 2: Read the compressed data from the output file and print it
    gzFile in_gz_file = gzopen(out_file_name, "rb");
    if (in_gz_file == NULL) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    char in_buf[1024];
    int read_count = gzread(in_gz_file, in_buf, sizeof(in_buf));
    while (read_count > 0) {
        printf("%.*s", read_count, in_buf);
        read_count = gzread(in_gz_file, in_buf, sizeof(in_buf));
    }
    
    gzclose(in_gz_file);
    
    // Step 3: Clean up
    assert_file_closed(&in_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}