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
//<ID> 1482
//<Prompt> ["TIFFWriteRawStrip","_TIFFfree","TIFFFlush","TIFFErrorExtR","TIFFGetField","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFReadScanline","_TIFFfree","TIFFClose"]
/*<Combination>: [tmsize_t TIFFWriteRawStrip(TIFF * tif, uint32_t strip, void * data, tmsize_t cc),
    void _TIFFfree(void * p),
    int TIFFFlush(TIFF * tif),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    void *_TIFFmalloc(tmsize_t s),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    int TIFFReadScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 90, nr_unique_branch: 31
//<Quality> {"density":27,"unique_branches":{"TIFFSetupStrips":[[209,34,209,71,10,0,4,1]],"TIFFWriteDirectorySec":[[694,25,697,62,0,0,4,0],[759,21,761,54,0,0,4,1],[1253,9,1253,20,0,0,4,0],[1255,9,1255,23,0,0,4,1],[209,34,209,71,61,0,4,0]],"TIFFAppendToStrip":[[767,9,767,29,0,0,4,1],[770,9,770,41,0,0,4,1],[770,45,770,65,0,0,4,1],[774,13,774,48,0,0,4,0],[775,13,775,45,0,0,4,1],[915,9,915,66,0,0,4,1]],"TIFFGrowStrips":[[734,9,734,32,0,0,4,1],[734,36,734,62,0,0,4,1]],"TIFFWriteDirectoryTagLongLong8Array":[[1955,17,1955,29,0,0,4,0]],"OJPEGDecode":[[863,9,863,40,0,0,4,0]],"TIFFWriteRawStrip":[[344,9,344,32,0,0,4,0],[344,9,344,32,0,0,4,1],[346,13,346,57,0,0,4,1],[358,13,358,43,0,0,4,0],[361,13,361,44,0,0,4,1],[365,9,365,35,0,0,4,0],[365,9,365,35,0,0,4,1],[373,9,373,35,0,0,4,0],[373,9,373,35,0,0,4,1],[379,13,379,63,0,0,4,0],[36,6,36,43,1,0,4,0],[36,47,36,79,1,0,4,0],[282,6,282,54,3,0,4,1]],"TIFFWriteDirectoryTagCheckedLong":[[2286,9,2286,35,0,0,4,0]],"TIFFFlush":[[51,9,51,35,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFReadScanline","TIFFErrorExtR","_TIFFfree","TIFFClose","TIFFClose","TIFFWriteRawStrip","TIFFErrorExtR","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFReadScanline","TIFFWriteRawStrip","TIFFErrorExtR","_TIFFfree","TIFFClose","TIFFClose"],"visited":2}
/*Here is the completed LLVMFuzzerTestOneInput_61 function that achieves the given event using the libtiff APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<115) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read TIFF image information
    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Allocate memory for output image
    tmsize_t scanlineSize = TIFFRasterScanlineSize64(in_tif);
    void* buf = _TIFFmalloc(scanlineSize);
    if (!buf) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read each scanline and write into output TIFF
    TIFF* out_tif = TIFFOpen("output_file", fuzz_str_2);
    if (!out_tif) {
        _TIFFfree(buf);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(out_tif, fuzz_uint32_t_3, height);
    TIFFSetField(out_tif, fuzz_uint32_t_4, 1);
    TIFFSetField(out_tif, fuzz_uint32_t_5, 8);
    TIFFSetField(out_tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    
    for (uint32_t row = 0; row < height; row++) {
        for (uint16_t sample = 0; sample < width; sample++) {
            if (TIFFReadScanline(in_tif, buf, fuzz_uint32_t_6, fuzz_uint16_t_7) != 1) {
                TIFFErrorExtR(in_tif, fuzz_str_8, fuzz_str_9);
                _TIFFfree(buf);
                TIFFClose(in_tif);
                TIFFClose(out_tif);
                assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
            }
            if (TIFFWriteRawStrip(out_tif, fuzz_uint32_t_10, buf, scanlineSize) != scanlineSize) {
                TIFFErrorExtR(out_tif, fuzz_str_11, fuzz_str_12);
                _TIFFfree(buf);
                TIFFClose(in_tif);
                TIFFClose(out_tif);
                assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
            }
        }
    }
    
    // Free memory and close TIFF files
    _TIFFfree(buf);
    TIFFClose(in_tif);
    TIFFClose(out_tif);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}
