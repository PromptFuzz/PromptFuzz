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
//<ID> 1710
//<Prompt> ["TIFFFieldSetGetCountSize","TIFFStreamOpen","TIFFVGetField","TIFFGetField","TIFFScanlineSize64","TIFFOpen","TIFFGetReadProc","TIFFSetField","_TIFFmalloc","TIFFReadEncodedStrip","TIFFWriteRawTile","_TIFFfree","TIFFClose"]
/*<Combination>: [int TIFFFieldSetGetCountSize(const TIFFField * ),
    TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFVGetField(TIFF * tif, uint32_t tag, __va_list_tag * ap),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    TIFFReadWriteProc TIFFGetReadProc(TIFF * ),
    int TIFFSetField(TIFF * , uint32_t ),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    tmsize_t TIFFWriteRawTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","TIFFClose","TIFFReadEncodedStrip","TIFFComputeStrip","_TIFFfree","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteRawTile","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","_TIFFmalloc","TIFFReadEncodedStrip","TIFFComputeStrip","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteRawTile","_TIFFfree","TIFFClose","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // Write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read input tiff information
    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    uint64_t scanlineSize = TIFFScanlineSize64(in_tif);

    // Allocate memory for image data
    uint8_t* imageBuffer = (uint8_t*)_TIFFmalloc(height * scanlineSize);
    if (imageBuffer == NULL) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read image data
    for (uint32_t row = 0; row < height; row++) {
        tmsize_t bytesRead = TIFFReadEncodedStrip(in_tif, TIFFComputeStrip(in_tif, fuzz_uint32_t_2, fuzz_uint16_t_3), imageBuffer + row * scanlineSize, scanlineSize);
        if (bytesRead < 0) {
            _TIFFfree(imageBuffer);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Create output tiff
    TIFF* out_tif = TIFFOpen("output_file", fuzz_str_4);
    if (!out_tif) {
        _TIFFfree(imageBuffer);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Set output tiff fields
    TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Write image data to output tiff
    for (uint32_t row = 0; row < height; row++) {
        tmsize_t bytesWritten = TIFFWriteRawTile(out_tif, fuzz_uint32_t_5, imageBuffer + row * scanlineSize, scanlineSize);
        if (bytesWritten < 0) {
            _TIFFfree(imageBuffer);
            TIFFClose(in_tif);
            TIFFClose(out_tif);
            assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
        }
    }

    // Cleanup
    _TIFFfree(imageBuffer);
    TIFFClose(in_tif);
    TIFFClose(out_tif);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}