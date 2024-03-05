#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 499
//<Prompt> ["inflatePrime","deflateBound","gzeof","inflateBack","gztell","inflateCopy","gzputs","deflateTune","deflateResetKeep"]
/*<Combination>: [int inflatePrime(z_streamp strm, int bits, int value),
    uLong deflateBound(z_streamp strm, uLong sourceLen),
    int gzeof(gzFile file),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    off_t gztell(gzFile ),
    int inflateCopy(z_streamp dest, z_streamp source),
    int gzputs(gzFile file, const char * s),
    int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain),
    int deflateResetKeep(z_streamp )
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"gzeof":[[476,9,476,21,0,0,4,0]]},"library_calls":["inflatePrime","deflateBound","gzeof","inflateBack","gztell","inflateCopy","gzopen","gzputs","deflateTune","deflateResetKeep","gzclose"],"critical_calls":["inflatePrime","deflateBound","gzeof","inflateBack","gztell","inflateCopy","gzopen","gzputs","deflateTune","deflateResetKeep","gzclose"],"visited":1}
/**/

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer for reading the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Define the input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";
    
    // Step 5: Get the file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    FILE *out_file = fopen(output_file, "wb");
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: Perform the desired event using zlib APIs
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Step 2: Call each API at least once
    int bits = 8;
    int value = 0;
    inflatePrime(&strm, bits, value);
    
    uLong sourceLen = 0;
    uLong bound = deflateBound(&strm, sourceLen);
    
    if (gzeof(NULL)) {
        // Do something
    }
    
    in_func in = NULL;
    void *in_desc = NULL;
    out_func out = NULL;
    void *out_desc = NULL;
    inflateBack(&strm, in, in_desc, out, out_desc);
    
    off_t offset = gztell(NULL);
    
    z_stream dest_strm;
    inflateCopy(&dest_strm, &strm);
    
    gzFile gz_file = gzopen(input_file, "wb");
    gzputs(gz_file, "testing");
    
    int good_length = 256;
    int max_lazy = 8;
    int nice_length = 32;
    int max_chain = 16;
    deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    
    deflateResetKeep(&strm);
    
    // Step 7: Release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    gzclose(gz_file);
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}