#include "FDSan.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 207
//<Prompt> ["gzclose_w","inflateSetDictionary","deflateEnd","inflatePrime","adler32_combine64","gzclearerr","gzdirect"]
/*<Combination>: [int gzclose_w(gzFile file),
    int inflateSetDictionary(z_streamp strm, const Bytef * dictionary, uInt dictLength),
    int deflateEnd(z_streamp strm),
    int inflatePrime(z_streamp strm, int bits, int value),
    uLong adler32_combine64(uLong , uLong , off64_t ),
    void gzclearerr(gzFile file),
    int gzdirect(gzFile file)
*/
//<score> 6, nr_unique_branch: 5
//<Quality> {"density":1,"unique_branches":{"gzclose_w":[[600,9,600,21,0,0,4,0]],"inflatePrime":[[227,9,227,18,0,0,4,0]],"gzdirect":[[563,9,563,21,0,0,4,0]],"adler32_combine_":[[150,9,150,21,0,0,4,1]],"gz_open":[[236,9,236,24,0,0,4,0]]},"library_calls":[],"critical_calls":[],"visited":0}
/**/


// Step 1: Achieve the event by using zlib APIs
void  id_000012_zlibEvent(const uint8_t *data, size_t size) {
    gzFile file = gzopen("input_file", "rb");
    
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));

    if (inflateInit(&strm) != Z_OK) {
        gzclose(file);
        return;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        gzclose(file);
        inflateEnd(&strm);
        return;
    }
    
    // Use the zlib APIs to achieve the event
    // ...

    gzclose_w(file);
    inflateSetDictionary(&strm, NULL, 0);
    deflateEnd(&strm);
    inflatePrime(&strm, 0, 0);
    adler32_combine64(0, 0, 0);
    gzclearerr(file);
    gzdirect(file);
    
    assert_file_closed(&out_file);;
    gzclose(file);
    inflateEnd(&strm);
}

// Step 2: Complete the LLVMFuzzerTestOneInput_12 function
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *data, size_t size) {
	if(size<0) return 0;

    // Step 3: Consume the input data using  id_000012_zlibEvent
     id_000012_zlibEvent(data, size);
    
    return 0;
}