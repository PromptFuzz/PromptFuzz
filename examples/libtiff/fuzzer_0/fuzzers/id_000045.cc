#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 627
//<Prompt> ["TIFFStreamOpen","TIFFCurrentStrip","_TIFFmalloc","_TIFFmemset","_TIFFfree","_TIFFmemset","TIFFSetField","TIFFOpen","TIFFGetSizeProc","_TIFFmalloc","TIFFReadRawTile","TIFFWriteTile","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint32_t TIFFCurrentStrip(TIFF * ),
    void *_TIFFmalloc(tmsize_t s),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    void _TIFFfree(void * p),
    void _TIFFmemset(void * p, int v, tmsize_t c),
    int TIFFSetField(TIFF * , uint32_t ),
    TIFF *TIFFOpen(const char * filename, const char * mode),
    TIFFSizeProc TIFFGetSizeProc(TIFF * ),
    void *_TIFFmalloc(tmsize_t s),
    tmsize_t TIFFReadRawTile(TIFF * tif, uint32_t tile, void * buf, tmsize_t size),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFGetSizeProc","TIFFClientdata","_TIFFmalloc","TIFFClose","TIFFReadRawTile","_TIFFfree","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFSetField","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetSizeProc","TIFFClientdata","_TIFFmalloc","TIFFReadRawTile","TIFFOpen","TIFFSetField","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* data, size_t size) {
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

    // Get the size of the input tiff
    TIFFSizeProc size_proc = TIFFGetSizeProc(in_tif);
    tmsize_t tiff_size = size_proc(TIFFClientdata(in_tif));

    // Allocate memory for the input tiff data
    void* tiff_data = _TIFFmalloc(tiff_size);
    if (!tiff_data) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the raw tile data from the input tiff
    tmsize_t read_size = TIFFReadRawTile(in_tif, 0, tiff_data, tiff_size);
    if (read_size < 0) {
        _TIFFfree(tiff_data);
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output tiff
    TIFF* out_tif = TIFFOpen("output_file", "w");
    if (!out_tif) {
        _TIFFfree(tiff_data);
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Set TIFF field
    TIFFSetField(out_tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

    // Write the tile data to the output tiff
    tmsize_t write_size = TIFFWriteTile(out_tif, tiff_data, 0, 0, 0, 0);
    if (write_size < 0) {
        _TIFFfree(tiff_data);
        TIFFClose(in_tif);
        TIFFClose(out_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Free allocated memory
    _TIFFfree(tiff_data);
    TIFFClose(in_tif);
    TIFFClose(out_tif);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}