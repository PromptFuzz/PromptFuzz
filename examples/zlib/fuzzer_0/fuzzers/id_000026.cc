#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 430
//<Prompt> ["zlibCompileFlags","gzdopen","gzclearerr","gzungetc","uncompress2","gzprintf","inflateBackInit_","inflateGetHeader","gzoffset","compress2","gzclose"]
/*<Combination>: [uLong zlibCompileFlags(),
    gzFile gzdopen(int fd, const char * mode),
    void gzclearerr(gzFile file),
    int gzungetc(int c, gzFile file),
    int uncompress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong * sourceLen),
    int gzprintf(gzFile file, const char * format),
    int inflateBackInit_(z_streamp strm, int windowBits, unsigned char * window, const char * version, int stream_size),
    int inflateGetHeader(z_streamp strm, gz_headerp head),
    off_t gzoffset(gzFile ),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    int gzclose(gzFile file)
*/
//<score> 10, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"gzvprintf":[[377,9,377,25,0,0,4,0],[377,29,377,49,0,0,4,1],[390,9,390,28,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","gzclearerr","gzungetc","uncompress2","gzopen","gzprintf","inflateBackInit_","zlibVersion","inflateGetHeader","gzoffset","compress2","gzclose","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzclearerr","gzungetc","uncompress2","gzopen","gzprintf","inflateBackInit_","zlibVersion","inflateGetHeader","gzoffset","compress2","gzclose","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE pointer to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");

    // Step 6: Define input and output file names
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 1: Call the zlib APIs to achieve the desired event
    uLong compile_flags = zlibCompileFlags();
	int fuzz_fd_0 = fuzz_fileno(in_file);

    gzFile gzfile = gzdopen(fuzz_fd_0, "rb");
    gzclearerr(gzfile);
    gzungetc(0, gzfile);

    // Step 2: Call the zlib APIs at least once
    Bytef *dest = new Bytef[size];
    uLongf destLen = size;
    const Bytef *source = (const Bytef *)data;
    uLong sourceLen = size;
    uncompress2(dest, &destLen, source, &sourceLen);

    gzFile gzoutput = gzopen(output_file, "wb");
    gzprintf(gzoutput, "Output data");

    z_stream stream;
    inflateBackInit_(&stream, 15, nullptr, zlibVersion(), sizeof(z_stream));
    gz_header header;
    inflateGetHeader(&stream, &header);

    off_t offset = gzoffset(gzfile);

    int level = 6;
    compress2(dest, &destLen, source, sourceLen, level);

    gzclose(gzfile);

    // Step 7: Release allocated resources
    assert_file_closed(&in_file);;
    delete[] dest;
    gzclose(gzoutput);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}