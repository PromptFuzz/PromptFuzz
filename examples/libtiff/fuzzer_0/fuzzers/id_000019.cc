#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 354
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
//<score> 132, nr_unique_branch: 12
//<Quality> {"density":12,"unique_branches":{"TIFFReadRawTile":[[1170,35,1170,64,0,0,4,1],[1174,9,1174,24,0,0,4,1]],"gtStripSeparate":[[1220,17,1220,31,0,0,4,0]],"putRGBcontig8bittile":[[1381,13,1381,19,1,0,4,1]],"gtTileSeparate":[[925,31,925,38,0,0,4,1],[934,16,934,25,0,0,4,1]],"PickSeparateCase":[[3118,44,3118,69,0,0,4,1]],"putRGBAAcontig8bittile":[[1381,13,1381,19,1,0,4,1]],"putRGBseparate8bittile":[[1381,13,1381,19,1,0,4,1]],"LogLuv24fromXYZ":[[1054,9,1054,12,0,0,4,0]],"gtStripContig":[[1097,17,1097,31,0,0,4,0]],"LogL10fromY":[[883,14,883,28,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFReadRGBAImageOriented","TIFFClose","TIFFIsByteSwapped","TIFFStripSize64","TIFFSetTagExtender","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","_TIFFfree","TIFFClose","LogLuv24fromXYZ","TIFFClose","_TIFFfree"],"critical_calls":["TIFFStreamOpen","TIFFReadRGBAImageOriented","TIFFIsByteSwapped","TIFFStripSize64","TIFFSetTagExtender","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","LogLuv24fromXYZ","TIFFClose","_TIFFfree"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size) {
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

    // Read the RGBA image information
    uint32_t width, height;
    uint32_t* raster;
    int result = TIFFReadRGBAImageOriented(in_tif, width, height, raster, 0, ORIENTATION_TOPLEFT);
    if (result == 0) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Check if the TIFF is byte swapped
    int is_byte_swapped = TIFFIsByteSwapped(in_tif);

    // Get the strip size of the TIFF
    uint64_t strip_size = TIFFStripSize64(in_tif);

    // Set a tag extender for the TIFF
    TIFFExtendProc extender = TIFFSetTagExtender(NULL);

    // Allocate memory for reading raw tile data
    tmsize_t tile_size = TIFFStripSize(in_tif);
    void* tile_data = _TIFFmalloc(tile_size);

    // Read raw tile data
    tmsize_t read_tile_size = TIFFReadRawTile(in_tif, 0, tile_data, tile_size);
    if (read_tile_size != tile_size) {
        _TIFFfree(tile_data);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Perform some calculations using LogLuv24fromXYZ
    float xyz[3];
    int result_xyz = LogLuv24fromXYZ(xyz, 0);

    // Close the input TIFF and free allocated memory
    TIFFClose(in_tif);
    _TIFFfree(tile_data);

    assert_file_closed(&in_file);
	return 0;
}