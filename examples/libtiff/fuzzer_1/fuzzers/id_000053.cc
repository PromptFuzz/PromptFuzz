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
//<ID> 1089
//<Prompt> ["TIFFStreamOpen","TIFFGetField","TIFFScanlineSize64","TIFFCreateCustomDirectory","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFReadEncodedStrip","TIFFOpen","TIFFSetField","TIFFWriteRawTile","_TIFFmemset","TIFFClose","_TIFFfree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    int TIFFCreateCustomDirectory(TIFF * , const TIFFFieldArray * ),
    void *_TIFFmalloc(tmsize_t s),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    tmsize_t TIFFReadEncodedStrip(TIFF * tif, uint32_t strip, void * buf, tmsize_t size),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    int TIFFSetField(TIFF * , uint32_t ),
    tmsize_t TIFFWriteRawTile(TIFF * tif, uint32_t tile, void * data, tmsize_t cc),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    void TIFFClose(TIFF * tif),
    void _TIFFfree(void * p)
*/
//<score> 5.4, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","TIFFOpen","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFClose","TIFFClose","TIFFReadEncodedStrip","_TIFFfree","TIFFClose","TIFFClose","TIFFWriteRawTile","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFScanlineSize64","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFReadEncodedStrip","TIFFWriteRawTile","_TIFFfree","TIFFClose","TIFFClose"],"visited":4}
/**/
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read TIFF tags
    uint32_t width = 0;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    
    uint32_t height = 0;
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);

    uint64_t scanline_size = TIFFScanlineSize64(in_tif);

    // Create an output TIFF file
    TIFF* out_tif = TIFFOpen("output_file", fuzz_str_2);
    if (!out_tif) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Set TIFF tags for output TIFF file
    TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out_tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    // Allocate memory for a scanline buffer
    void* scanline_buffer = _TIFFmalloc(scanline_size);
    if (!scanline_buffer) {
        TIFFClose(in_tif);
        TIFFClose(out_tif);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Read and write each scanline
    for (uint32_t i = 0; i < height; i++) {
        tmsize_t bytes_read = TIFFReadEncodedStrip(in_tif, fuzz_uint32_t_3, scanline_buffer, scanline_size);
        if (bytes_read < 0) {
            _TIFFfree(scanline_buffer);
            TIFFClose(in_tif);
            TIFFClose(out_tif);
            assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
        }

        tmsize_t bytes_written = TIFFWriteRawTile(out_tif, fuzz_uint32_t_4, scanline_buffer, bytes_read);
        if (bytes_written < 0) {
            _TIFFfree(scanline_buffer);
            TIFFClose(in_tif);
            TIFFClose(out_tif);
            assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
        }
    }

    // Free scanline buffer
    _TIFFfree(scanline_buffer);

    // Close TIFF files
    TIFFClose(in_tif);
    TIFFClose(out_tif);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}