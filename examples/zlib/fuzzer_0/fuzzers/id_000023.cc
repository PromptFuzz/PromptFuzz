#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 411
//<Prompt> ["deflateCopy","compress2","gzseek","gzgetc","gzbuffer","deflateParams","crc32_z"]
/*<Combination>: [int deflateCopy(z_streamp dest, z_streamp source),
    int compress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen, int level),
    off_t gzseek(gzFile , off_t , int ),
    int gzgetc(gzFile file),
    int gzbuffer(gzFile file, unsigned int size),
    int deflateParams(z_streamp strm, int level, int strategy),
    uLong crc32_z(uLong crc, const Bytef * buf, z_size_t len)
*/
//<score> 8, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"gzseek64":[[356,31,356,49,0,0,4,0]]},"library_calls":["deflateCopy","compress2","gzopen","gzseek","gzclose","gzopen","gzclose","gzopen","gzbuffer","gzclose","deflateParams","crc32_z"],"critical_calls":["deflateCopy","compress2","gzopen","gzseek","gzclose","gzopen","gzclose","gzopen","gzbuffer","gzclose","deflateParams","crc32_z"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 6: Specify input and output file names
    const char* input_file = "input_file";
    const char* output_file = "output_file";

    // Step 5: Get file descriptors for reading and writing
    int in_fd = fuzz_fileno(in_file);
    if (in_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    FILE *out_file = fopen(output_file, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    int out_fd = fuzz_fileno(out_file);
    if (out_fd == -1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 2: Call zlib APIs to achieve the specified event
    z_stream stream;
    stream.next_in = NULL;
    stream.avail_in = 0;
    stream.total_in = 0;
    stream.next_out = NULL;
    stream.avail_out = 0;
    stream.total_out = 0;
    stream.msg = NULL;
    stream.state = NULL;
    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.opaque = NULL;
    stream.data_type = 0;
    stream.adler = 0;
    stream.reserved = 0;

    // deflateCopy
    z_stream source;
    source.next_in = NULL;
    source.avail_in = 0;
    source.total_in = 0;
    source.next_out = NULL;
    source.avail_out = 0;
    source.total_out = 0;
    source.msg = NULL;
    source.state = NULL;
    source.zalloc = NULL;
    source.zfree = NULL;
    source.opaque = NULL;
    source.data_type = 0;
    source.adler = 0;
    source.reserved = 0;

    deflateCopy(&stream, &source);

    // compress2
    Bytef *deflate_buffer = new Bytef[size];
    uLongf deflate_buffer_size = size;
    int compression_level = Z_DEFAULT_COMPRESSION;
    compress2(deflate_buffer, &deflate_buffer_size, (const Bytef *)data, size, compression_level);

    // gzseek
    gzFile gzfile = gzopen(input_file, "rb");
    if (!gzfile) {
        delete[] deflate_buffer;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    off_t offset = gzseek(gzfile, 0L, SEEK_END);
    gzclose(gzfile);

    // gzgetc
    gzfile = gzopen(input_file, "rb");
    if (!gzfile) {
        delete[] deflate_buffer;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    int c = gzgetc(gzfile);
    gzclose(gzfile);

    // gzbuffer
    gzfile = gzopen(input_file, "rb");
    if (!gzfile) {
        delete[] deflate_buffer;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    unsigned int buffer_size = 1024;
    gzbuffer(gzfile, buffer_size);
    gzclose(gzfile);

    // deflateParams
    int deflate_level = Z_DEFAULT_COMPRESSION;
    int deflate_strategy = Z_DEFAULT_STRATEGY;
    deflateParams(&stream, deflate_level, deflate_strategy);

    // crc32_z
    uLong crc_value = crc32_z(0L, (const Bytef *)data, size);

    // Step 7: Release allocated resources
    delete[] deflate_buffer;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}