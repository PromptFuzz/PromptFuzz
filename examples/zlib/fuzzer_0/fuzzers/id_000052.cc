#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 981
//<Prompt> ["gzgetc","gzopen","inflateSyncPoint","inflateMark","gzclose"]
/*<Combination>: [int gzgetc(gzFile file),
    gzFile gzopen(const char * , const char * ),
    int inflateSyncPoint(z_streamp ),
    long inflateMark(z_streamp strm),
    int gzclose(gzFile file)
*/
//<score> 4, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"inflateSyncPoint":[[1434,9,1434,32,0,0,4,1],[1436,12,1436,33,0,0,4,1]]},"library_calls":["gzopen","gzopen","inflateInit_","inflateSyncPoint","inflateMark","gzputc","inflateEnd","gzclose","gzclose"],"critical_calls":["gzopen","gzopen","inflateInit_","inflateSyncPoint","inflateMark","gzputc","inflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create input and output file streams
    FILE *in_file = fmemopen((void *)data, size, "rb");
    FILE *out_file = fopen("output_file", "wb");
    if (in_file == NULL || out_file == NULL) {
        // Error handling
        if (in_file != NULL) {
            assert_file_closed(&in_file);;
        }
        if (out_file != NULL) {
            assert_file_closed(&out_file);;
        }
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 5: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Step 2: Call zlib APIs
    gzFile input = gzopen("input_file", "rb");
    gzFile output = gzopen("output_file", "wb");
    z_stream strm;
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    inflateInit(&strm);
    
    // Step 1: Event - Read input byte stream and write output after inflating
    int c;
    while ((c = gzgetc(input)) != EOF) {
        inflateSyncPoint(&strm);
        long pos = inflateMark(&strm);
        gzputc(output, (char)c);
    }
    
    inflateEnd(&strm);
    gzclose(input);
    gzclose(output);
    
    // Step 7: Release resources
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