#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 346
//<Prompt> ["TIFFVTileSize","TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmalloc","TIFFFieldWithTag","TIFFYCbCrtoRGB","_TIFFmemset","TIFFStripSize","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"]
/*<Combination>: [tmsize_t TIFFVTileSize(TIFF * tif, uint32_t nrows),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFCurrentStrip(TIFF * ),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    const TIFFField *TIFFFindField(TIFF * , uint32_t , TIFFDataType ),
    int TIFFFieldPassCount(const TIFFField * ),
    void *_TIFFmalloc(tmsize_t s),
    const TIFFField *TIFFFieldWithTag(TIFF * , uint32_t ),
    void TIFFYCbCrtoRGB(TIFFYCbCrToRGB * , uint32_t , int32_t , int32_t , uint32_t * , uint32_t * , uint32_t * ),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    tmsize_t TIFFStripSize(TIFF * tif),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 15.428572, nr_unique_branch: 24
//<Quality> {"density":18,"unique_branches":{"_tiffReadProc":[[70,9,70,38,0,0,4,1],[76,26,76,50,0,0,4,0],[76,26,76,50,0,0,4,1],[80,13,80,34,0,0,4,1],[83,13,83,23,0,0,4,0],[83,13,83,23,0,0,4,1],[86,9,86,18,0,0,4,1]],"TIFFReadRawStrip":[[700,9,700,31,0,0,4,0],[700,9,700,31,0,0,4,1],[702,9,702,32,0,0,4,1],[709,9,709,40,0,0,4,0],[709,9,709,40,0,0,4,1],[717,9,717,31,0,0,4,0],[717,35,717,64,0,0,4,0],[717,35,717,64,0,0,4,1],[721,9,721,24,0,0,4,0],[721,9,721,24,0,0,4,1]],"TIFFWriteRawStrip":[[36,6,36,43,1,0,4,1],[36,47,36,79,1,0,4,1]],"_TIFFgetMode":[[62,9,62,17,0,0,4,0],[64,17,64,31,0,0,4,1]],"TIFFClientOpenExt":[[596,9,596,17,0,0,4,1],[634,9,634,17,0,0,4,0],[640,17,640,43,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFClose","TIFFReadRawStrip","_TIFFfree","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFWriteRawStrip","_TIFFfree","TIFFClose","TIFFClose","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFReadRawStrip","TIFFOpen","TIFFWriteRawStrip","TIFFClose","_TIFFfree","TIFFClose"],"visited":6}
/**/
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
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

    // Get the number of strips in the input TIFF
    uint32_t numStrips = TIFFNumberOfStrips(in_tif);

    // Iterate through each strip
    for (uint32_t strip = 0; strip < numStrips; strip++) {
        // Get the size of the current strip
        tmsize_t stripSize = TIFFStripSize(in_tif);

        // Allocate memory for the strip data
        void* stripData = _TIFFmalloc(stripSize);
        if (!stripData) {
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        // Read the raw strip data
        tmsize_t bytesRead = TIFFReadRawStrip(in_tif, strip, stripData, stripSize);
        if (bytesRead < 0) {
            _TIFFfree(stripData);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        // Process the strip data (e.g. perform some operations on it)
        // ...

        // Write the processed strip data to an output TIFF
        TIFF* out_tif = TIFFOpen("output_file", "a");
        if (!out_tif) {
            _TIFFfree(stripData);
            TIFFClose(in_tif);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
        }
        if (TIFFWriteRawStrip(out_tif, strip, stripData, stripSize) < 0) {
            _TIFFfree(stripData);
            TIFFClose(in_tif);
            TIFFClose(out_tif);
            assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
        }
        
        // Close the output TIFF
        TIFFClose(out_tif);

        // Free memory for the strip data
        _TIFFfree(stripData);
    }

    // Close the input TIFF
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}