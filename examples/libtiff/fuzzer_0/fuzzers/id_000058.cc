#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1179
//<Prompt> ["TIFFStreamOpen","TIFFGetField","TIFFCreateDirectory","TIFFRasterScanlineSize64","TIFFError","_TIFFmalloc","TIFFReadEncodedTile","TIFFGetReadProc","TIFFComputeTile","TIFFOpen","TIFFWriteEncodedTile","TIFFClose","_TIFFfree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    int TIFFCreateDirectory(TIFF * ),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    void TIFFError(const char * , const char * ),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadEncodedTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    TIFFReadWriteProc TIFFGetReadProc(TIFF * ),
    uint32_t TIFFComputeTile(TIFF * tif, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    tmsize_t TIFFWriteEncodedTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void TIFFClose(TIFF * tif),
    void _TIFFfree(void * p)
*/
//<score> 60, nr_unique_branch: 5
//<Quality> {"density":30,"unique_branches":{"TIFFStartTile":[[1460,9,1460,23,0,0,4,0],[282,6,282,54,4,0,4,1]],"TIFFComputeTile":[[52,9,52,16,0,0,4,1]],"TIFFError":[[49,9,49,26,0,0,4,0],[55,9,55,29,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFError","TIFFClose","TIFFGetField","TIFFError","TIFFClose","TIFFOpen","TIFFError","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFError","TIFFClose","TIFFClose","TIFFReadEncodedTile","TIFFComputeTile","TIFFError","_TIFFfree","TIFFClose","TIFFClose","TIFFWriteEncodedTile","TIFFComputeTile","TIFFError","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFReadEncodedTile","TIFFComputeTile","TIFFWriteEncodedTile","TIFFComputeTile","TIFFError","_TIFFfree","TIFFClose","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

     // Write data into input file
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

     uint32_t width;
     uint32_t height;

     // Get the image width and height
     if (TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width) == 0) {
         TIFFError("Error", "Failed to retrieve image width");
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	return 0;
     }
     if (TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height) == 0) {
         TIFFError("Error", "Failed to retrieve image height");
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	return 0;
     }

     // Create output TIFF
     TIFF* out_tif = TIFFOpen("output_file", "w");
     if (!out_tif) {
         TIFFError("Error", "Failed to create output TIFF");
         TIFFClose(in_tif);
         assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
     }

     // Set output TIFF properties
     TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
     TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
     TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
     TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 3);
     TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
     TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

     // Allocate memory buffer for a single row
     tmsize_t row_size = TIFFRasterScanlineSize64(in_tif);
     void* row_buffer = _TIFFmalloc(row_size);
     if (!row_buffer) {
         TIFFError("Error", "Failed to allocate memory for row buffer");
         TIFFClose(in_tif);
         TIFFClose(out_tif);
         assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
     }

     // Read and write each row of the image
     for (uint32_t row = 0; row < height; row++) {
         // Read a row from the input TIFF
         if (TIFFReadEncodedTile(in_tif, TIFFComputeTile(in_tif, 0, row, 0, 0), row_buffer, row_size) < 0) {
             TIFFError("Error", "Failed to read row from input TIFF");
             _TIFFfree(row_buffer);
             TIFFClose(in_tif);
             TIFFClose(out_tif);
             assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
         }

         // Write the row to the output TIFF
         if (TIFFWriteEncodedTile(out_tif, TIFFComputeTile(out_tif, 0, row, 0, 0), row_buffer, row_size) < 0) {
             TIFFError("Error", "Failed to write row to output TIFF");
             _TIFFfree(row_buffer);
             TIFFClose(in_tif);
             TIFFClose(out_tif);
             assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
         }
     }

     // Free row buffer memory
     _TIFFfree(row_buffer);

     // Close input and output TIFFs
     TIFFClose(in_tif);
     TIFFClose(out_tif);

     assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
 }