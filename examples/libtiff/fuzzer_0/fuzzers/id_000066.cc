#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1849
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfDirectories","LogL16toY","TIFFReadRGBAStripExt","LogLuv24fromXYZ","TIFFTileSize","TIFFRasterScanlineSize64","TIFFScanlineSize64","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFUnlinkDirectory","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    tdir_t TIFFNumberOfDirectories(TIFF * ),
    double LogL16toY(int ),
    int TIFFReadRGBAStripExt(TIFF * , uint32_t , uint32_t * , int stop_on_error),
    uint32_t LogLuv24fromXYZ(float * , int ),
    tmsize_t TIFFTileSize(TIFF * tif),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFReadScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    void _TIFFfree(void * p),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    void TIFFClose(TIFF * tif)
*/
//<score> 30, nr_unique_branch: 7
//<Quality> {"density":10,"unique_branches":{"putRGBAAseparate8bittile":[[1354,9,1354,15,4,0,4,0]],"gtStripContig":[[1080,13,1080,24,0,0,4,0]],"putRGBseparate8bittile":[[1346,9,1346,15,4,0,4,1],[1354,9,1354,15,4,0,4,0]],"setorientation":[[675,9,675,34,0,0,4,0]],"TIFFReadRGBAStripExt":[[3224,9,3224,35,0,0,4,1],[3225,9,3225,59,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","_TIFFfree","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAStripExt","_TIFFfree","_TIFFfree","TIFFClose"],"visited":1}
/*Here's the complete implementation of the fuzz driver using the provided libtiff APIs:

*/


// Include the libtiff headers

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

  // Write data into input_file
  FILE* in_file = fopen("input_file", "wb");
  if (!in_file) {
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

  // Get the number of directories in the TIFF file
  tdir_t numDirectories = TIFFNumberOfDirectories(in_tif);

  // Iterate through each directory
  for (tdir_t dirIndex = 0; dirIndex < numDirectories; dirIndex++) {
    // Set the current directory
    if (!TIFFSetDirectory(in_tif, dirIndex)) {
      continue;  // Skip to the next directory if the current one is invalid
    }

    // Read RGB strips from the TIFF file
    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
    if (raster != nullptr) {
      // Read the RGB strips into the raster buffer
      if (TIFFReadRGBAStripExt(in_tif, 0, raster, 1) != 1) {
        _TIFFfree(raster);
        continue;  // Skip to the next directory if reading fails
      }

      // Perform some operations on the raster buffer
      for (uint32_t i = 0; i < width * height; i++) {
        uint32_t pixel = raster[i];
        // Perform operations on the pixel value
      }

      // Free the raster buffer
      _TIFFfree(raster);
    }
  }

  // Close the input TIFF file
  TIFFClose(in_tif);

  assert_file_closed(&in_file);
	return 0;
}