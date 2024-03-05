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
//<ID> 447
//<Prompt> ["deflateReset","inflateCopy","gzdirect","gzputs","deflateParams","gztell"]
/*<Combination>: [int deflateReset(z_streamp strm),
    int inflateCopy(z_streamp dest, z_streamp source),
    int gzdirect(gzFile file),
    int gzputs(gzFile file, const char * s),
    int deflateParams(z_streamp strm, int level, int strategy),
    off_t gztell(gzFile )
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["deflateReset","inflateCopy","gzdirect","gzputs","deflateParams","gztell","deflateEnd","inflateEnd","gzclose"],"critical_calls":["deflateReset","inflateCopy","gzdirect","gzputs","deflateParams","gztell","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





  // Step 4: Create input and output file variables
  FILE *in_file = fmemopen((void *)data, size, "rb");
  FILE *out_file = fopen("output_file", "wb");
  
  // Step 5: Get file descriptors for input and output files
  int in_fd = fuzz_fileno(in_file);
  int out_fd = fuzz_fileno(out_file);
  
  // Step 6: Specify file names
  const char *in_file_name = "input_file";
  const char *out_file_name = "output_file";
  
  // Step 1: Initialize zlib structures and variables
  z_stream strm;
  z_stream source_strm;
  gzFile gzfile;
  int zlib_result;
  char buffer[1024];
  
  // Step 1: Call zlib APIs to achieve the event
  
  // int deflateReset(z_streamp strm)
  deflateReset(&strm);
  
  // int inflateCopy(z_streamp dest, z_streamp source)
  inflateCopy(&strm, &source_strm);
  
  // int gzdirect(gzFile file)
  int is_direct = gzdirect(gzfile);
  
  // int gzputs(gzFile file, const char * s)
  gzputs(gzfile, fuzz_str_1);
  
  // int deflateParams(z_streamp strm, int level, int strategy)
  deflateParams(&strm, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
  
  // off_t gztell(gzFile file)
  off_t current_pos = gztell(gzfile);
  
  // Step 7: Release allocated resources
  deflateEnd(&strm);
  inflateEnd(&strm);
  gzclose(gzfile);
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