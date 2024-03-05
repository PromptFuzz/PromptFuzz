#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1060
//<Prompt> ["deflateBound","deflateInit_","gzread","uncompress2","gzgetc","gzclose"]
/*<Combination>: [uLong deflateBound(z_streamp strm, uLong sourceLen),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int uncompress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong * sourceLen),
    int gzgetc(gzFile file),
    int gzclose(gzFile file)
*/
//<score> 9.333333, nr_unique_branch: 6
//<Quality> {"density":14,"unique_branches":{"gz_read":[[308,40,308,62,0,0,4,1],[318,18,318,36,0,0,4,0],[318,18,318,36,0,0,4,1],[319,17,319,66,0,0,4,1],[327,17,327,39,0,0,4,0],[327,17,327,39,0,0,4,1]]},"library_calls":["deflateBound","deflateInit_","gzopen","deflateEnd","gzread","gzclose","deflateEnd","deflate","gzclose","deflateEnd","gzclose","deflateEnd","gzclose","deflateEnd"],"critical_calls":["deflateBound","deflateInit_","gzopen","gzread","deflate","gzclose","deflateEnd"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 1: Declare necessary variables
    z_stream strm;
    gzFile file;
    Bytef *source = NULL;
    Bytef *dest = NULL;
    uLong sourceLen = size;
    uLongf destLen = deflateBound(&strm, sourceLen);

    // Step 2: Initialize zlib and gzip file
    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    file = gzopen("input_file", "rb");
    if (file == NULL) {
        deflateEnd(&strm);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Step 3: Allocate memory buffers
    source = (Bytef*)malloc(sourceLen);
    dest = (Bytef*)malloc(destLen);

    // Step 4: Read input data from file
    int bytesRead = gzread(file, source, sourceLen);
    if (bytesRead != sourceLen) {
        free(dest);
        free(source);
        gzclose(file);
        deflateEnd(&strm);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Step 5: Compress the input data
    strm.next_in = source;
    strm.avail_in = sourceLen;
    strm.next_out = dest;
    strm.avail_out = destLen;
    
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        free(dest);
        free(source);
        gzclose(file);
        deflateEnd(&strm);
        assert_file_name_closed("input_file");
	return 0;
    }

    // Step 6: Write the compressed data to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        free(dest);
        free(source);
        gzclose(file);
        deflateEnd(&strm);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return 0;
    }
    fwrite(dest, 1, strm.total_out, out_file);
    assert_file_closed(&out_file);;

    // Step 7: Release resources
    free(dest);
    free(source);
    gzclose(file);
    deflateEnd(&strm);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	return 0;
}