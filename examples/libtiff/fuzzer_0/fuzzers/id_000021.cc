#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 356
//<Prompt> ["TIFFStreamOpen","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImageOriented","TIFFStripSize64","TIFFIsByteSwapped","TIFFSetTagExtender","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","LogLuv24fromXYZ","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFReadRGBAImageOriented(TIFF * , uint32_t , uint32_t , uint32_t * , int , int ),
    uint64_t TIFFStripSize64(TIFF * tif),
    int TIFFIsByteSwapped(TIFF * ),
    TIFFExtendProc TIFFSetTagExtender(TIFFExtendProc ),
    tmsize_t TIFFStripSize(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    uint32_t LogLuv24fromXYZ(float * , int ),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFClose","TIFFReadRGBAImageOriented","_TIFFfree","TIFFClose","TIFFStripSize64","_TIFFmalloc","_TIFFfree","TIFFClose","TIFFReadRawTile","LogLuv24fromXYZ","_TIFFfree","_TIFFfree","TIFFClose","_TIFFfree","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImageOriented","TIFFStripSize64","_TIFFmalloc","TIFFReadRawTile","LogLuv24fromXYZ","_TIFFfree","_TIFFfree","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Declare variables
    uint32_t width, height;
    uint32_t* raster;
    tmsize_t stripSize;
    void* stripBuffer;
    uint32_t orientation;
    
    // Read TIFF header
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Allocate memory for raster
    tmsize_t rasterSize = width * height * sizeof(uint32_t);
    raster = (uint32_t*)_TIFFmalloc(rasterSize);
    if (!raster) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read RGBA image
    if (TIFFReadRGBAImageOriented(in_tif, width, height, raster, 0, 1) == 0) {
        _TIFFfree(raster);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Determine strip size
    stripSize = TIFFStripSize64(in_tif);
    
    // Allocate memory for strip buffer
    stripBuffer = _TIFFmalloc(stripSize);
    if (!stripBuffer) {
        _TIFFfree(raster);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read raw tile data
    tmsize_t tileSize = stripSize;
    TIFFReadRawTile(in_tif, 0, stripBuffer, tileSize);
    
    // Convert LogLuv24 to XYZ
    float xyz[3];
    int bitsPerSample = 8 * sizeof(uint32_t);
    int logLuv24 = LogLuv24fromXYZ(xyz, bitsPerSample);
    
    // Write output data to file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file == NULL) {
        _TIFFfree(raster);
        _TIFFfree(stripBuffer);
        TIFFClose(in_tif);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    fwrite(stripBuffer, sizeof(uint8_t), stripSize, out_file);
    assert_file_closed(&out_file);;
    
    // Release allocated resources
    _TIFFfree(raster);
    _TIFFfree(stripBuffer);
    TIFFClose(in_tif);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}