#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 324
//<Prompt> ["gzoffset","gzseek64","gzfread","zlibVersion","inflateBackInit_","gzclose_r","inflateInit2_"]
/*<Combination>: [off_t gzoffset(gzFile ),
    off64_t gzseek64(gzFile , off64_t , int ),
    z_size_t gzfread(voidp buf, z_size_t size, z_size_t nitems, gzFile file),
    const char *zlibVersion(),
    int inflateBackInit_(z_streamp strm, int windowBits, unsigned char * window, const char * version, int stream_size),
    int gzclose_r(gzFile file),
    int inflateInit2_(z_streamp strm, int windowBits, const char * version, int stream_size)
*/
//<score> 7.5, nr_unique_branch: 9
//<Quality> {"density":5,"unique_branches":{"gzseek64":[[356,9,356,27,0,0,4,0],[356,31,356,49,0,0,4,1],[360,9,360,27,0,0,4,1],[362,14,362,25,0,0,4,1]],"gzfread":[[387,9,387,31,0,0,4,1],[388,14,388,32,0,0,4,1],[393,9,393,13,0,0,4,0],[393,17,393,37,0,0,4,1],[399,12,399,15,0,0,4,0]]},"library_calls":["gzopen","gzoffset","gzseek64","gzfread","zlibVersion","inflateBackInit_","gzclose_r","inflateInit2_","inflateEnd"],"critical_calls":["gzopen","gzoffset","gzseek64","gzfread","zlibVersion","inflateBackInit_","gzclose_r","inflateInit2_","inflateEnd"],"visited":1}
/*Here is the C++ program that achieves the event using the zlib library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    int ret = 0;
    gzFile file = gzopen("input_file", "rb");
    if (file == NULL) {
        // handle error
        assert_file_name_closed("input_file");
	return ret;
    }

    gzoffset(file);
    off64_t offset = gzseek64(file, 0, SEEK_CUR);
    if (offset == -1) {
        // handle error
    }

    uint8_t buffer[1024];
    z_size_t bytesRead = gzfread(buffer, sizeof(uint8_t), sizeof(buffer), file);
    if (bytesRead == 0) {
        // handle error
    }

    const char *version = zlibVersion();

    z_stream stream;
    stream.next_in = NULL;
    stream.avail_in = 0;
    stream.total_in = 0;
    stream.next_out = NULL;
    stream.avail_out = 0;
    stream.total_out = 0;
    stream.msg = NULL;
    stream.state = NULL;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.data_type = 0;
    stream.adler = 0;
    stream.reserved = 0;

    ret = inflateBackInit_(&stream, 15, NULL, version, sizeof(z_stream));
    if (ret != Z_OK) {
        // handle error
    }

    gzclose_r(file);

    ret = inflateInit2_(&stream, 15, version, sizeof(z_stream));
    if (ret != Z_OK) {
        // handle error
    }

    inflateEnd(&stream);

    assert_file_name_closed("input_file");
	return ret;
}