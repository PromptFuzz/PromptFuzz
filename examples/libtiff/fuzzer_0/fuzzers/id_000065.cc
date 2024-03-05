#include "FDSan.h"
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


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* data, size_t size) {
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

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    tdir_t num_dirs = TIFFNumberOfDirectories(in_tif);
    TIFFSetDirectory(in_tif, 0);

    for (tdir_t dir = 0; dir < num_dirs; dir++) {
        TIFFSetDirectory(in_tif, dir);

        uint32_t scanline_size = TIFFRasterScanlineSize64(in_tif);
        void* scanline_buffer = _TIFFmalloc(scanline_size);
        if (!scanline_buffer) {
            TIFFErrorExtR(in_tif, "LLVMFuzzerTestOneInput_65", "Failed to allocate scanline buffer");
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        for (uint32_t row = 0; row < TIFFScanlineSize(in_tif); row++) {
            if (TIFFReadScanline(in_tif, scanline_buffer, row, 0) == -1) {
                TIFFErrorExtR(in_tif, "LLVMFuzzerTestOneInput_65", "Failed to read scanline");
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