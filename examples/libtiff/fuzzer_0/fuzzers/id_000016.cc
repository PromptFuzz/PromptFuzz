#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 335
//<Prompt> ["TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmemset","TIFFStreamOpen","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFCurrentStrip(TIFF * ),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    const TIFFField *TIFFFindField(TIFF * , uint32_t , TIFFDataType ),
    int TIFFFieldPassCount(const TIFFField * ),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    tmsize_t TIFFStripSize(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 36, nr_unique_branch: 5
//<Quality> {"density":12,"unique_branches":{"TIFFReadRawTile":[[1155,9,1155,31,0,0,4,0]],"tagCompare":[[554,16,554,44,0,0,4,1]],"TIFFWriteCheck":[[602,9,602,34,0,0,4,0]],"TIFFCheckTile":[[88,9,88,31,0,0,4,0]],"TIFFWriteTile":[[390,9,390,40,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmalloc","_TIFFmemset","TIFFStripSize","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFCurrentStrip","TIFFUnlinkDirectory","TIFFFindField","TIFFFieldPassCount","_TIFFmalloc","_TIFFmemset","TIFFStripSize","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"],"visited":1}
/*The completed version of the fuzz driver is as follows:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* data, size_t size) {
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

  // Open input TIFF in memory
  std::istringstream s(std::string(data, data + size));
  TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
  if (!in_tif) {
    assert_file_closed(&in_file);
	return 0;
  }

  // Get the current strip number
  uint32_t current_strip = TIFFCurrentStrip(in_tif);

  // Unlink a directory
  TIFFUnlinkDirectory(in_tif, current_strip);

  // Find a field in the TIFF file
  const TIFFField* field = TIFFFindField(in_tif, TIFFTAG_IMAGEWIDTH, TIFF_LONG);

  // Get the pass count of a field
  int pass_count = TIFFFieldPassCount(field);

  // Set a buffer to a specific value
  char* buffer = (char*)_TIFFmalloc(size);
  _TIFFmemset(buffer, 0, size);

  // Strip size of the TIFF file
  tmsize_t strip_size = TIFFStripSize(in_tif);

  // Read a raw tile from the TIFF file
  tmsize_t read_size = TIFFReadRawTile(in_tif, current_strip, buffer, strip_size);

  // Write a tile to the TIFF file
  tmsize_t write_size = TIFFWriteTile(in_tif, buffer, 0, 0, 0, 0);

  // Free allocated memory
  _TIFFfree(buffer);

  // Close the TIFF file
  TIFFClose(in_tif);

  assert_file_closed(&in_file);
	return 0;
}