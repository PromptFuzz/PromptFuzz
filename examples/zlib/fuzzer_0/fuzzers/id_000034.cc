#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 550
//<Prompt> ["gzdopen","gzgetc","get_crc_table","inflateCodesUsed","inflateBackEnd","adler32_combine64","gzfwrite","crc32_combine_op","gzclose"]
/*<Combination>: [gzFile gzdopen(int fd, const char * mode),
    int gzgetc(gzFile file),
    const z_crc_t *get_crc_table(),
    unsigned long inflateCodesUsed(z_streamp ),
    int inflateBackEnd(z_streamp strm),
    uLong adler32_combine64(uLong , uLong , off64_t ),
    z_size_t gzfwrite(voidpc buf, z_size_t size, z_size_t nitems, gzFile file),
    uLong crc32_combine_op(uLong crc1, uLong crc2, uLong op),
    int gzclose(gzFile file)
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":3,"unique_branches":{},"library_calls":["gzdopen","get_crc_table","inflateCodesUsed","inflateBackEnd","adler32_combine64","gzfwrite","crc32_combine_op","gzclose"],"critical_calls":["gzdopen","get_crc_table","inflateCodesUsed","inflateBackEnd","adler32_combine64","gzfwrite","crc32_combine_op","gzclose"],"visited":0}
/**/

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* data, size_t size) {
	if(size<0) return 0;

    // Step 4: Open input file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 6: Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 4: Open output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 6: Get file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    
    // Step 1: gzdopen
    gzFile in_gzfile = gzdopen(in_fd, "rb");
    if (!in_gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Step 2: gzgetc
    int c = gzgetc(in_gzfile);
    
    // Step 1: get_crc_table
    const z_crc_t* crc_table = get_crc_table();

    // Step 1: inflateCodesUsed
    z_stream strm;
    strm.avail_in = size;
    strm.next_in = (Bytef*)data;
    unsigned long codes_used = inflateCodesUsed(&strm);

    // Step 1: inflateBackEnd
    inflateBackEnd(&strm);

    // Step 1: adler32_combine64
    uLong adler1 = 1;
    uLong adler2 = 2;
    off64_t offset = 0;
    uLong combined_adler = adler32_combine64(adler1, adler2, offset);

    // Step 1: gzfwrite
    z_size_t items_written = gzfwrite(data, sizeof(uint8_t), size, in_gzfile);

    // Step 1: crc32_combine_op
    uLong crc1 = 1;
    uLong crc2 = 2;
    uLong op = 3;
    uLong combined_crc = crc32_combine_op(crc1, crc2, op);

    // Step 7: gzclose
    gzclose(in_gzfile);
    
    // Step 7: Close and release resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}