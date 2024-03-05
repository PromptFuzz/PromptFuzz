#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1167
//<Prompt> ["TIFFStreamOpen","TIFFGetField","TIFFScanlineSize64","_TIFFrealloc","TIFFWriteCheck","TIFFOpen","TIFFSetField","_TIFFmalloc","TIFFFlush","TIFFReadEncodedStrip","TIFFWriteRawTile","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    void *_TIFFrealloc(void * p, tmsize_t s),
    int TIFFWriteCheck(TIFF * , int , const char * ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    int TIFFSetField(TIFF * , uint32_t ),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFFlush(TIFF * tif),
    tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    tmsize_t TIFFWriteRawTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFScanlineSize64","_TIFFmalloc","TIFFClose","TIFFGetField","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFClose","_TIFFfree"],"critical_calls":["TIFFStreamOpen","TIFFScanlineSize64","_TIFFmalloc","TIFFGetField","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFClose","_TIFFfree"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size) {
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

  // Get the scanline size of the input TIFF
  uint64_t scanlineSize64 = TIFFScanlineSize64(in_tif);

  // Allocate memory for a scanline buffer
  void* scanlineBuffer = _TIFFmalloc(scanlineSize64);
  if (!scanlineBuffer) {
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	return 0;
  }

  // Read and process each scanline of the input TIFF
  uint32_t height = 0;
  TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
  for (uint32_t row = 0; row < height; row++) {
    tmsize_t bytesRead = TIFFReadEncodedStrip(
        in_tif, TIFFComputeStrip(in_tif, row, 0), scanlineBuffer, scanlineSize64);
    if (bytesRead < 0) {
      break;
    }
    // Process the scanline here...
  }

  // Close the input TIFF and free the scanline buffer
  TIFFClose(in_tif);
  _TIFFfree(scanlineBuffer);

  assert_file_closed(&in_file);
	return 0;
}