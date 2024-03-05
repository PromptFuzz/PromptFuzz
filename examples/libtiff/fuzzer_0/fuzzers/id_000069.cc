#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1915
//<Prompt> ["TIFFGetStrileOffset","TIFFStreamOpen","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","_TIFFmemset","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFOpen","TIFFSetField","TIFFFlushData","TIFFWriteRawTile","_TIFFfree","TIFFClose"]
/*<Combination>: [uint64_t TIFFGetStrileOffset(TIFF * tif, uint32_t strile),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    uint32_t TIFFComputeStrip(TIFF * , uint32_t , uint16_t ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    int TIFFSetField(TIFF * , uint32_t ),
    int TIFFFlushData(TIFF * tif),
    tmsize_t TIFFWriteRawTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFNumberOfStrips","TIFFReadEncodedStrip","_TIFFfree","TIFFClose","TIFFClose","TIFFWriteRawStrip","_TIFFfree","TIFFClose","TIFFClose","TIFFFlushData","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFOpen","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFWriteRawStrip","TIFFFlushData","_TIFFfree","TIFFClose","TIFFClose"],"visited":1}
/*Here is the complete C++ program that implements the fuzz driver using the libtiff library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);

    // Read a strip from the input tiff and write it to output tiff
    uint32_t stripSize = TIFFStripSize(in_tif);
    void* stripBuffer = _TIFFmalloc(stripSize);
    if (stripBuffer == nullptr) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    TIFF* out_tif = TIFFOpen("output_file", "w");
    if (!out_tif) {
        _TIFFfree(stripBuffer);
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    uint32_t stripCount = TIFFNumberOfStrips(in_tif);
    for (uint32_t stripIndex = 0; stripIndex < stripCount; stripIndex++) {
        tmsize_t bytesRead = TIFFReadEncodedStrip(in_tif, stripIndex, stripBuffer, stripSize);
        if (bytesRead < 0) {
            _TIFFfree(stripBuffer);
            TIFFClose(out_tif);
            TIFFClose(in_tif);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
        }
        tmsize_t bytesWritten = TIFFWriteRawStrip(out_tif, stripIndex, stripBuffer, bytesRead);
        if (bytesWritten < 0) {
            _TIFFfree(stripBuffer);
            TIFFClose(out_tif);
            TIFFClose(in_tif);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
        }
    }

    TIFFFlushData(out_tif);

    _TIFFfree(stripBuffer);
    TIFFClose(out_tif);
    TIFFClose(in_tif);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}