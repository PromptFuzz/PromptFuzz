#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 776
//<Prompt> ["TIFFStreamOpen","TIFFOpen","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","_TIFFmalloc","TIFFReadEncodedTile","TIFFComputeTile","TIFFWriteEncodedTile","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    TIFFUnmapFileProc TIFFGetUnmapFileProc(TIFF * ),
    double LogL10toY(int ),
    void TIFFPrintDirectory(TIFF * , FILE * , long ),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadEncodedTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    uint32_t TIFFComputeTile(TIFF * tif, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    tmsize_t TIFFWriteEncodedTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 8, nr_unique_branch: 2
//<Quality> {"density":4,"unique_branches":{"TIFFPrintDirectory":[[298,17,298,34,0,0,4,0],[336,13,336,44,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFClose","TIFFPrintDirectory","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFPrintDirectory","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file.
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // print directory
    FILE *out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        TIFFClose(in_tif);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    TIFFPrintDirectory(in_tif, out_file, 0);
    assert_file_closed(&out_file);;

    TIFFClose(in_tif);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}