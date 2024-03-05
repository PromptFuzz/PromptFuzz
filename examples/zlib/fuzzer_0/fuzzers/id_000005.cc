#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 71
//<Prompt> ["gzrewind","inflatePrime","gzopen","gzdirect","gztell64","inflateMark"]
/*<Combination>: [int gzrewind(gzFile file),
    int inflatePrime(z_streamp strm, int bits, int value),
    gzFile gzopen(const char * , const char * ),
    int gzdirect(gzFile file),
    off64_t gztell64(gzFile ),
    long inflateMark(z_streamp strm)
*/
//<score> 5, nr_unique_branch: 3
//<Quality> {"density":5,"unique_branches":{"inflateMark":[[1513,9,1513,32,0,0,4,0]],"inflate":[[611,10,611,33,0,0,4,0],[611,37,611,56,0,0,4,1]]},"library_calls":["gzdopen","gzrewind","inflateInit2_","inflate","inflateEnd","gztell64","inflatePrime","inflateMark","gzdirect","gzclose"],"critical_calls":["gzdopen","gzrewind","inflateInit2_","inflate","inflateEnd","gztell64","inflatePrime","inflateMark","gzdirect","gzclose"],"visited":2}
/**/

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  // Create input file from input data
  FILE *in_file = fmemopen((void *)data, size, "rb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Create output file
  FILE *out_file = fopen("output_file", "wb");
  if (out_file == NULL) {
    assert_file_closed(&in_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
  }

  // Get file descriptors
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);

  // Open gzip file for reading
  gzFile gzfile = gzdopen(in_fd, "rb");
  if (gzfile == NULL) {
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
  }

  // Rewind the gzip file
  int rewind_result = gzrewind(gzfile);

  // Inflate the gzip data
  z_stream strm;
  strm.next_in = NULL;
  strm.avail_in = 0;
  inflateInit2(&strm, 15 + 32); // Use default windowBits value
  unsigned char buffer[4096];
  strm.next_out = buffer;
  strm.avail_out = sizeof(buffer);
  int inflate_result = inflate(&strm, Z_NO_FLUSH);
  if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
    // Write inflated data to output file
    fwrite(buffer, sizeof(unsigned char), sizeof(buffer) - strm.avail_out, out_file);
  }
  inflateEnd(&strm);

  // Get current position in the gzip file
  off64_t tell_result = gztell64(gzfile);

  // Mark current position in the inflate stream
  inflatePrime(&strm, 256, 256);
  long mark_result = inflateMark(&strm);

  // Check if the gzip file is in direct mode
  int direct_mode = gzdirect(gzfile);

  // Close the gzip file and release resources
  gzclose(gzfile);
  assert_file_closed(&in_file);;
  assert_file_closed(&out_file);;

  assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}