#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1095
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
//<score> 10, nr_unique_branch: 2
//<Quality> {"density":5,"unique_branches":{"deflate":[[1210,24,1210,40,0,0,4,0],[1215,21,1215,42,0,0,4,1]]},"library_calls":["gzopen","gzopen64","gzeof","gzread","gzbuffer","gzputc","deflateSetDictionary","deflateTune","inflateSync","gzflush","inflateGetHeader","gzclose","gzclose"],"critical_calls":["gzopen","gzopen64","gzeof","gzread","gzbuffer","gzputc","deflateSetDictionary","deflateTune","inflateSync","gzflush","inflateGetHeader","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create FILE pointers to read input data and write output data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file == nullptr) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 1: Open gzFile
    gzFile input_gzfile = gzopen("input_file", "rb");
    gzFile output_gzfile = gzopen64("output_file", "wb");

    if (input_gzfile == nullptr || output_gzfile == nullptr) {
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

    // Step 6: Use file names directly
    const char *input_file = "input_file";
    const char *output_file = "output_file";

    // Step 2: Use zlib APIs to achieve the desired event
    while (!gzeof(input_gzfile)) {
        char buffer[1024];
        int read_size = gzread(input_gzfile, buffer, sizeof(buffer));
        if (read_size <= 0) {
            break;
        }

        gzbuffer(output_gzfile, 1024);
        gzputc(output_gzfile, 'A');

        write(out_fd, buffer, read_size);
    }

    // Step 3: Use zlib APIs to consume input data and size
    z_stream strm;
    memset(&strm, 0, sizeof(strm));

    deflateSetDictionary(&strm, (const Bytef *)data, size);
    deflateTune(&strm, 256, 128, 16, 4);
    inflateSync(&strm);
    gzflush(output_gzfile, Z_SYNC_FLUSH);

    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm, &header);

    // Step 7: Release all allocated resources
    gzclose(input_gzfile);
    gzclose(output_gzfile);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}