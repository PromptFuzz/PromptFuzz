#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<268) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_20, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_21, fuzz_str_sz_21, fdp);
	//fuzzer shim end}




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
     TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
     if (!in_tif) {
         assert_file_closed(&in_file);
	return 0;
     }

     uint32_t width;
     uint32_t height;

     // Get the image width and height
     if (TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width) == 0) {
         TIFFError(fuzz_str_2, fuzz_str_3);
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	return 0;
     }
     if (TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height) == 0) {
         TIFFError(fuzz_str_4, fuzz_str_5);
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	return 0;
     }

     // Create output TIFF
     TIFF* out_tif = TIFFOpen("output_file", fuzz_str_6);
     if (!out_tif) {
         TIFFError(fuzz_str_7, fuzz_str_8);
         TIFFClose(in_tif);
         assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
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
         TIFFError(fuzz_str_9, fuzz_str_10);
         TIFFClose(in_tif);
         TIFFClose(out_tif);
         assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
     }

     // Read and write each row of the image
     for (uint32_t row = 0; row < height; row++) {
         // Read a row from the input TIFF
         if (TIFFReadEncodedTile(in_tif, TIFFComputeTile(in_tif, fuzz_uint32_t_11, fuzz_uint32_t_12, fuzz_uint32_t_13, fuzz_uint16_t_14), row_buffer, row_size) < 0) {
             TIFFError(fuzz_str_15, fuzz_str_16);
             _TIFFfree(row_buffer);
             TIFFClose(in_tif);
             TIFFClose(out_tif);
             assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
         }

         // Write the row to the output TIFF
         if (TIFFWriteEncodedTile(out_tif, TIFFComputeTile(out_tif, fuzz_uint32_t_17, fuzz_uint32_t_18, fuzz_uint32_t_19, fuzz_uint16_t_20), row_buffer, row_size) < 0) {
             TIFFError(fuzz_str_21, "Failed to write row to output TIFF");
             _TIFFfree(row_buffer);
             TIFFClose(in_tif);
             TIFFClose(out_tif);
             assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
         }
     }

     // Free row buffer memory
     _TIFFfree(row_buffer);

     // Close input and output TIFFs
     TIFFClose(in_tif);
     TIFFClose(out_tif);

     assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
 }