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
//<ID> 1085
//<Prompt> ["gzfread","compress","inflateBack","inflateEnd"]
/*<Combination>: [z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file),
    int compress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    int inflateBack(z_streamp strm, in_func in, void * in_desc, out_func out, void * out_desc),
    int inflateEnd(z_streamp strm)
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzopen","gzclose","gzclose","gzread","gzclose","compressBound","gzclose","compress","gzclose","inflateBack","gzclose"],"critical_calls":["gzopen","gzread","compressBound","compress","inflateBack","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
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
    if (file == NULL) {
        assert_file_name_closed("input_file");
	return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        gzclose(file);
        assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        assert_file_closed(&in_file);;
        gzclose(file);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Read the input data from the file
    const int BUFSIZE = 1024;
    char buffer[BUFSIZE];
    int read_len;
    do {
        read_len = gzread(file, buffer, BUFSIZE);
        if (read_len > 0) {
            fwrite(buffer, read_len, 1, out_file);
        }
    } while (read_len == BUFSIZE);

    // Compress the input data and write to the output file
    fseek(in_file, 0L, SEEK_END);
    long in_size = ftell(in_file);
    fseek(in_file, 0L, SEEK_SET);
    Bytef *in_data = (Bytef *)malloc(in_size);
    if (in_data == NULL) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        gzclose(file);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    fread(in_data, in_size, 1, in_file);

    uLongf out_size = compressBound(in_size);
    Bytef *out_data = (Bytef *)malloc(out_size);
    if (out_data == NULL) {
        free(in_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        gzclose(file);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    compress(out_data, &out_size, in_data, in_size);
    fwrite(out_data, out_size, 1, out_file);

    // Decompress the output data and write to the output file
    fseek(out_file, 0L, SEEK_END);
    long out_size2 = ftell(out_file);
    fseek(out_file, 0L, SEEK_SET);
    Bytef *out_data2 = (Bytef *)malloc(out_size2);
    if (out_data2 == NULL) {
        free(in_data);
        free(out_data);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        gzclose(file);
        assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    fread(out_data2, out_size2, 1, out_file);

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = out_size2;
    strm.next_in = out_data2;
    strm.avail_out = in_size;
    strm.next_out = in_data;

    inflateBack(&strm, Z_NULL, Z_NULL, Z_NULL, Z_NULL);
    fwrite(in_data, in_size - strm.avail_out, 1, out_file);

    free(in_data);
    free(out_data);
    free(out_data2);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    gzclose(file);

    assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}