#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 681
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFFileName","TIFFGetSeekProc","TIFFCIELabToXYZ","TIFFGetField","TIFFTileSize","TIFFRasterScanlineSize64","TIFFScanlineSize64","TIFFReadScanline","_TIFFfree","TIFFUnlinkDirectory","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    tdir_t TIFFNumberOfDirectories(TIFF * ),
    int TIFFSetDirectory(TIFF * , tdir_t ),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    const char *TIFFFileName(TIFF * ),
    TIFFSeekProc TIFFGetSeekProc(TIFF * ),
    void TIFFCIELabToXYZ(TIFFCIELabToRGB * , uint32_t , int32_t , int32_t , float * , float * , float * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    tmsize_t TIFFTileSize(TIFF * tif),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    uint64_t TIFFScanlineSize64(TIFF * tif),
    int TIFFReadScanline(TIFF * tif, void * buf, uint32_t row, uint16_t sample),
    void _TIFFfree(void * p),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    void TIFFClose(TIFF * tif)
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFClose","TIFFFileName","TIFFGetSeekProc","TIFFClose","TIFFTileSize","TIFFRasterScanlineSize64","TIFFScanlineSize64","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose","_TIFFfree","TIFFUnlinkDirectory","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFFileName","TIFFGetSeekProc","TIFFTileSize","TIFFRasterScanlineSize64","TIFFScanlineSize64","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFUnlinkDirectory","TIFFClose"],"visited":1}
/*The complete fuzz driver is as follows:

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of directories in the TIFF file
    tdir_t numDirectories = TIFFNumberOfDirectories(in_tif);

    // Iterate through each directory
    for (tdir_t i = 0; i < numDirectories; i++) {
        // Set the current directory
        if (!TIFFSetDirectory(in_tif, i)) {
            printf("Failed to set directory %d\n", i);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        // Get the current directory filename
        const char* filename = TIFFFileName(in_tif);
        printf("Filename: %s\n", filename);

        // Get the seek proc for the TIFF file
        TIFFSeekProc seekProc = TIFFGetSeekProc(in_tif);
        if (!seekProc) {
            printf("Failed to get seek proc\n");
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }

        // Get the tile size
        tmsize_t tileSize = TIFFTileSize(in_tif);
        printf("Tile size: %ld\n", tileSize);

        // Get the raster scanline size
        uint64_t rasterScanlineSize = TIFFRasterScanlineSize64(in_tif);
        printf("Raster scanline size: %lu\n", rasterScanlineSize);

        // Get the scanline size
        uint64_t scanlineSize = TIFFScanlineSize64(in_tif);
        printf("Scanline size: %lu\n", scanlineSize);

        // Read a scanline from the TIFF file
        uint32_t row = 0;
        uint16_t sample = 0;
        uint8_t* buf = (uint8_t*) _TIFFmalloc(scanlineSize);
        if (!TIFFReadScanline(in_tif, buf, row, sample)) {
            printf("Failed to read scanline\n");
            _TIFFfree(buf);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }
        _TIFFfree(buf);

        // Unlink the current directory
        if (!TIFFUnlinkDirectory(in_tif, i)) {
            printf("Failed to unlink directory %d\n", i);
            TIFFClose(in_tif);
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // Close the TIFF file
    TIFFClose(in_tif);

    // remove the input file
    remove("input_file");

    assert_file_name_closed("input_file");
	assert_file_closed(&in_file);
	return 0;
}