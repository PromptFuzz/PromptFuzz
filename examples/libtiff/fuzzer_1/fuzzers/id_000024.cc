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
//<ID> 375
//<Prompt> ["TIFFWriteScanline","TIFFGetClientInfo","TIFFSetErrorHandlerExt","TIFFTileRowSize64","TIFFRasterScanlineSize","TIFFSwabLong8","TIFFGetMapFileProc","TIFFScanlineSize64","TIFFFieldSetGetSize"]
/*<Combination>: [int TIFFWriteScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    void *TIFFGetClientInfo(TIFF * , const char * ),
    TIFFErrorHandlerExt TIFFSetErrorHandlerExt(TIFFErrorHandlerExt ),
    uint64_t TIFFTileRowSize64(TIFF * tif),
    tmsize_t TIFFRasterScanlineSize(TIFF * tif),
    void TIFFSwabLong8(uint64_t * ),
    TIFFMapFileProc TIFFGetMapFileProc(TIFF * ),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    int TIFFFieldSetGetSize(const TIFFField * )
*/
//<score> 19.5, nr_unique_branch: 4
//<Quality> {"density":13,"unique_branches":{"TIFFGetClientInfo":[[69,12,69,26,0,0,4,1],[72,9,72,23,0,0,4,1]],"TIFFWriteScanline":[[36,6,36,43,1,0,4,1],[36,47,36,79,1,0,4,1]]},"library_calls":["TIFFStreamOpen","_TIFFmalloc","TIFFClose","TIFFSetErrorHandlerExt","TIFFWriteScanline","TIFFGetClientInfo","TIFFSetErrorHandlerExt","TIFFTileRowSize64","TIFFRasterScanlineSize","TIFFSwabLong8","TIFFGetMapFileProc","TIFFScanlineSize64","TIFFFindField","TIFFFieldSetGetSize","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","_TIFFmalloc","TIFFSetErrorHandlerExt","TIFFWriteScanline","TIFFGetClientInfo","TIFFSetErrorHandlerExt","TIFFTileRowSize64","TIFFRasterScanlineSize","TIFFSwabLong8","TIFFGetMapFileProc","TIFFScanlineSize64","TIFFFindField","TIFFFieldSetGetSize","_TIFFfree","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<27) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // write data into input_file
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Initialize variables
    uint32_t rows_per_strip = 16;
    uint32_t width = 256;
    uint32_t height = 256;
    uint32_t buffer_size = width * height;
    uint32_t *buffer = (uint32_t *)_TIFFmalloc(buffer_size * sizeof(uint32_t));
    if (!buffer) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Set error handler
    TIFFErrorHandlerExt previous_handler = TIFFSetErrorHandlerExt(NULL);

    // Write scanline
    for (uint32_t row = 0; row < height; row++) {
        if (TIFFWriteScanline(in_tif, buffer, fuzz_uint32_t_2, fuzz_uint16_t_3) < 0) {
            break;
        }
    }

    // Get client info
    void *client_info = TIFFGetClientInfo(in_tif, fuzz_str_4);

    // Set new error handler
    TIFFSetErrorHandlerExt(previous_handler);

    // Get tile row size
    uint64_t tile_row_size = TIFFTileRowSize64(in_tif);

    // Get raster scanline size
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(in_tif);

    // Swab long 8
    uint64_t value = 123456789;
    TIFFSwabLong8(&value);

    // Get map file proc
    TIFFMapFileProc map_file_proc = TIFFGetMapFileProc(in_tif);

    // Get scanline size
    uint64_t scanline_size = TIFFScanlineSize64(in_tif);

    // Get field set size
    const TIFFField *field = TIFFFindField(in_tif, fuzz_uint32_t_5, TIFF_ASCII);
    int field_set_size = TIFFFieldSetGetSize(field);

    // Cleanup
    _TIFFfree(buffer);
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}