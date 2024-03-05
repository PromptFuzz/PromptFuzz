#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 294
//<Prompt> ["deflateParams","deflateSetHeader","inflateCodesUsed","deflateInit2_","inflateBackEnd","get_crc_table"]
/*<Combination>: [int deflateParams(z_streamp strm, int level, int strategy),
    int deflateSetHeader(z_streamp strm, gz_headerp head),
    unsigned long inflateCodesUsed(z_streamp ),
    int deflateInit2_(z_streamp strm, int level, int method, int windowBits, int memLevel, int strategy, const char * version, int stream_size),
    int inflateBackEnd(z_streamp strm),
    const z_crc_t *get_crc_table()
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"inflate":[[629,38,629,52,0,0,4,1],[640,17,640,35,0,0,4,0]]},"library_calls":["deflateInit2_","deflate","deflateEnd","inflateInit2_","inflate","inflateEnd","inflateCodesUsed","get_crc_table"],"critical_calls":["deflateInit2_","deflate","deflateEnd","inflateInit2_","inflate","inflateEnd","inflateCodesUsed","get_crc_table"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
	if(size<0) return 0;

  // Step 1: Event description
  // Description: Read the input data, deflate it using zlib, inflate it back using zlib, and check if the code lengths used in the inflate process. Finally, release all allocated resources.
  
  // Step 2: Deflate the input data
  z_stream deflate_strm;
  deflate_strm.zalloc = Z_NULL;
  deflate_strm.zfree = Z_NULL;
  deflate_strm.opaque = Z_NULL;
  
  deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
  
  deflate_strm.next_in = (Bytef *)data;
  deflate_strm.avail_in = size;
  
  unsigned char deflate_buffer[1024];
  
  do {
    deflate_strm.next_out = deflate_buffer;
    deflate_strm.avail_out = sizeof(deflate_buffer);
    
    deflate(&deflate_strm, Z_FINISH);
    
    // Step 4: Write the deflated data to an output file
    FILE *out_file = fopen("output_file", "wb");
    fwrite(deflate_buffer, 1, sizeof(deflate_buffer) - deflate_strm.avail_out, out_file);
    assert_file_closed(&out_file);;
  } while (deflate_strm.avail_out == 0);
  
  deflateEnd(&deflate_strm);
  
  // Step 3: Inflate the deflated data
  z_stream inflate_strm;
  inflate_strm.zalloc = Z_NULL;
  inflate_strm.zfree = Z_NULL;
  inflate_strm.opaque = Z_NULL;
  
  inflateInit2_(&inflate_strm, MAX_WBITS + 16, ZLIB_VERSION, sizeof(z_stream));
  
  inflate_strm.next_in = deflate_buffer;
  inflate_strm.avail_in = sizeof(deflate_buffer);
  
  unsigned char inflate_buffer[1024];
  
  do {
    inflate_strm.next_out = inflate_buffer;
    inflate_strm.avail_out = sizeof(inflate_buffer);
    
    inflate(&inflate_strm, Z_FINISH);
    
    // Step 4: Write the inflated data to an output file
    FILE *out_file = fopen("output_file", "ab");
    fwrite(inflate_buffer, 1, sizeof(inflate_buffer) - inflate_strm.avail_out, out_file);
    assert_file_closed(&out_file);;
  } while (inflate_strm.avail_out == 0);
  
  inflateEnd(&inflate_strm);
  
  // Step 5: Check the code lengths
  unsigned long code_lengths = inflateCodesUsed(&inflate_strm);
  
  // Step 6: Get the CRC table
  const z_crc_t *crc_table = get_crc_table();
  
  // Step 7: Release resources
  remove("output_file");
  
  assert_file_name_closed("output_file");
	return 0;
}