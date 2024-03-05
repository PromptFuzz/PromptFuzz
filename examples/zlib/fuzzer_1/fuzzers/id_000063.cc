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
//<ID> 1064
//<Prompt> ["deflateBound","deflateInit_","gzread","uncompress2","gzgetc","gzclose"]
/*<Combination>: [uLong deflateBound(z_streamp strm, uLong sourceLen),
    int deflateInit_(z_streamp strm, int level, const char * version, int stream_size),
    int gzread(gzFile file, voidp buf, unsigned int len),
    int uncompress2(Bytef * dest, uLongf * destLen, const Bytef * source, uLong * sourceLen),
    int gzgetc(gzFile file),
    int gzclose(gzFile file)
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["deflateBound","deflateInit_","deflate","deflateEnd","gzdopen","gzread","gzclose","uncompress2","gzdopen","gzclose"],"critical_calls":["deflateBound","deflateInit_","deflate","deflateEnd","gzdopen","gzread","gzclose","uncompress2","gzdopen","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Step 1: Deflate the input data
    uLongf compressedSize = deflateBound(NULL, size);
    Bytef *compressedData = new Bytef[compressedSize];
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    if (deflateInit_(&strm, fuzz_int32_t_2, fuzz_str_1, sizeof(z_stream)) == Z_OK) {
        strm.avail_in = size;
        strm.next_in = (Bytef *)data;
        strm.avail_out = compressedSize;
        strm.next_out = compressedData;

        deflate(&strm, Z_FINISH);
        deflateEnd(&strm);
    }
	FILE* fuzz_file_3 = fmemopen(compressedData, compressedSize, "rb");

	int fuzz_fd_2 = fuzz_fileno(fuzz_file_3);


    // Step 2: Create a gzFile variable to read the compressed data
    gzFile file = gzdopen(fuzz_fd_2, fuzz_str_3);

    if (file != NULL) {
        // Step 3: Read the compressed data
        const int bufferSize = 1024;
        char buffer[bufferSize];
        int bytesRead;

        while ((bytesRead = gzread(file, buffer, bufferSize)) > 0) {
            // Process the compressed data
        }

        gzclose(file);
    }

    // Step 4: Uncompress the compressed data
    uLongf uncompressedSize = size;
    Bytef *uncompressedData = new Bytef[uncompressedSize];

    if (uncompress2(uncompressedData, &uncompressedSize, compressedData, &compressedSize) == Z_OK) {
        // Process the uncompressed data
    }
	FILE* fuzz_file_1 = fmemopen(uncompressedData, uncompressedSize, "rb");

	int fuzz_fd_0 = fuzz_fileno(fuzz_file_1);


    // Step 5: Create a gzFile variable to read the uncompressed data
    file = gzdopen(fuzz_fd_0, fuzz_str_4);

    if (file != NULL) {
        // Step 6: Read the uncompressed data
        int character;
        while ((character = gzgetc(file)) != -1) {
            // Process the uncompressed data
        }

        gzclose(file);
    }

    // Step 7: Delete allocated resources
    delete[] compressedData;
    delete[] uncompressedData;

    assert_file_closed(&fuzz_file_1);
	assert_file_closed(&fuzz_file_3);
	assert_fd_closed(fuzz_fd_0);
	assert_fd_closed(fuzz_fd_2);
	return 0;
}