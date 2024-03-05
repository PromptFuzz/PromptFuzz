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
//<ID> 188
//<Prompt> ["compress","gzgets","inflateSyncPoint","crc32_combine","zError","crc32_combine64"]
/*<Combination>: [int compress(Bytef * dest, uLongf * destLen, const Bytef * source, uLong sourceLen),
    char *gzgets(gzFile file, char * buf, int len),
    int inflateSyncPoint(z_streamp ),
    uLong crc32_combine(uLong , uLong , off_t ),
    const char *zError(int ),
    uLong crc32_combine64(uLong , uLong , off64_t )
*/
//<score> 14, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"x2nmodp":[[180,12,180,13,0,0,4,0],[181,13,181,18,0,0,4,0],[181,13,181,18,0,0,4,1]]},"library_calls":["compressBound","compress","gzdopen","gzgets","inflateSyncPoint","crc32","crc32","crc32_combine","zError","crc32","crc32_combine64","gzclose"],"critical_calls":["compressBound","compress","gzdopen","gzgets","inflateSyncPoint","crc32","crc32","crc32_combine","zError","crc32","crc32_combine64","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




  // Step 4: Create a FILE * variable to read the input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (!in_file) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Step 5: Get the file descriptor for reading
  int in_fd = fuzz_fileno(in_file);

  // Step 6: Create a string for the output file name
  const char *out_filename = "output_file";

  // Step 4: Create a FILE * variable to write output data
  FILE *out_file = fopen(out_filename, "wb");
  if (!out_file) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 5: Get the file descriptor for writing
  int out_fd = fuzz_fileno(out_file);

  // Step 1: Compress the input data using zlib
  uLongf compressed_size = compressBound(size);
  Bytef *compressed_data = new Bytef[compressed_size];
  if (compress(compressed_data, &compressed_size, data, size) != Z_OK) {
    delete[] compressed_data;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Step 2: Read data from gzFile using gzgets
  gzFile gz_file = gzdopen(in_fd, fuzz_str_1);
  if (!gz_file) {
    delete[] compressed_data;
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  char buf[1024];
  gzgets(gz_file, buf, sizeof(buf));

  // Step 3: Use inflateSyncPoint
  z_stream zstream;
  zstream.next_in = compressed_data;
  zstream.avail_in = compressed_size;
  int result = inflateSyncPoint(&zstream);

  // Step 1: Use crc32_combine
  uLong crc1 = crc32(fuzz_uint64_t_2, data, size);
  uLong crc2 = crc32(fuzz_uint64_t_3, compressed_data, compressed_size);
  uLong combined_crc = crc32_combine(crc1, crc2, size);

  // Step 3: Use zError
  const char *error_msg = zError(result);

  // Step 6: Use crc32_combine64
  uLong crc3 = crc32(fuzz_uint64_t_4, compressed_data, compressed_size);
  uLong combined_crc64 = crc32_combine64(crc2, crc3, size);

  // Step 7: Clean up
  delete[] compressed_data;
  gzclose(gz_file);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}