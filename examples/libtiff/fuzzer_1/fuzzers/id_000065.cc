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
//<ID> 1809
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFGetField","_TIFFmemcmp","_TIFFmalloc","TIFFReadCustomDirectory","TIFFRasterScanlineSize64","TIFFReadScanline","_TIFFfree","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    tdir_t TIFFNumberOfDirectories(TIFF * ),
    int TIFFSetDirectory(TIFF * , tdir_t ),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    int _TIFFmemcmp(const void * p1, const void * p2, tmsize_t c),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFReadCustomDirectory(TIFF * tif, toff_t diroff, const TIFFFieldArray * infoarray),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    int TIFFReadScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    void _TIFFfree(void * p),
    void TIFFOpenOptionsSetMaxSingleMemAlloc(TIFFOpenOptions * opts, tmsize_t max_single_mem_alloc),
    void TIFFClose(TIFF * tif)
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFSetDirectory","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFErrorExtR","TIFFClose","TIFFScanlineSize","TIFFReadScanline","TIFFErrorExtR","_TIFFfree","TIFFClose","_TIFFfree","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFSetDirectory","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFScanlineSize","TIFFReadScanline","TIFFErrorExtR","_TIFFfree","TIFFClose"],"visited":0}
/*Here is the completed C++ program for the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<111) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
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

    tdir_t num_dirs = TIFFNumberOfDirectories(in_tif);
    TIFFSetDirectory(in_tif, fuzz_uint32_t_2);

    for (tdir_t dir = 0; dir < num_dirs; dir++) {
        TIFFSetDirectory(in_tif, fuzz_uint32_t_3);

        uint32_t scanline_size = TIFFRasterScanlineSize64(in_tif);
        void* scanline_buffer = _TIFFmalloc(scanline_size);
        if (!scanline_buffer) {
            TIFFErrorExtR(in_tif, fuzz_str_4, fuzz_str_5);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        for (uint32_t row = 0; row < TIFFScanlineSize(in_tif); row++) {
            if (TIFFReadScanline(in_tif, scanline_buffer, fuzz_uint32_t_6, fuzz_uint16_t_7) == -1) {
                TIFFErrorExtR(in_tif, fuzz_str_8, "Failed to read scanline");
                if (scanline_buffer) {
                    _TIFFfree(scanline_buffer);
                }
                TIFFClose(in_tif);
                assert_file_closed(&in_file);
	return 0;
            }
        }

        if (scanline_buffer) {
            _TIFFfree(scanline_buffer);
        }
    }

    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}