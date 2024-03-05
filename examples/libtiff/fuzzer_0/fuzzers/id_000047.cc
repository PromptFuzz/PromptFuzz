#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 684
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
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"OJPEGReadHeaderInfoSecTablesQTable":[[2057,17,2057,35,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFFileName","TIFFGetField","TIFFErrorExtR","TIFFFileName","TIFFTileSize","TIFFScanlineSize64","_TIFFmalloc","TIFFErrorExtR","TIFFFileName","TIFFReadScanline","TIFFErrorExtR","TIFFFileName","_TIFFfree","TIFFUnlinkDirectory","TIFFCurrentDirectory","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFFileName","TIFFGetField","TIFFErrorExtR","TIFFFileName","TIFFTileSize","TIFFScanlineSize64","_TIFFmalloc","TIFFErrorExtR","TIFFFileName","TIFFReadScanline","TIFFErrorExtR","TIFFFileName","_TIFFfree","TIFFUnlinkDirectory","TIFFCurrentDirectory","TIFFClose"],"visited":3}
/**/
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file.
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of directories in the input TIFF
    tdir_t numDirectories = TIFFNumberOfDirectories(in_tif);

    // Iterate through each directory
    for (tdir_t dir = 0; dir < numDirectories; dir++) {
        // Set the current directory
        if (!TIFFSetDirectory(in_tif, dir)) {
            TIFFErrorExtR(in_tif, TIFFFileName(in_tif), "Error setting directory");
            continue;
        }

        // Get the tag data for the current directory
        uint32_t tagData;
        if (!TIFFGetField(in_tif, TIFFTAG_SAMPLESPERPIXEL, &tagData)) {
            TIFFErrorExtR(in_tif, TIFFFileName(in_tif), "Error getting field");
            continue;
        }

        // Get the tile size for the current directory
        tmsize_t tileSize = TIFFTileSize(in_tif);

        // Get the scanline size for the current directory
        uint64_t scanlineSize = TIFFScanlineSize64(in_tif);

        // Allocate memory for reading a scanline or tile
        void* buffer = _TIFFmalloc(scanlineSize);
        if (!buffer) {
            TIFFErrorExtR(in_tif, TIFFFileName(in_tif), "Error allocating memory");
            continue;
        }

        // Read the scanline data for each sample
        for (uint16_t sample = 0; sample < tagData; sample++) {
            if (TIFFReadScanline(in_tif, buffer, 0, sample) == -1) {
                TIFFErrorExtR(in_tif, TIFFFileName(in_tif), "Error reading scanline");
                continue;
            }
        }

        // Free the allocated memory
        _TIFFfree(buffer);
    }

    // Unlink the current directory
    TIFFUnlinkDirectory(in_tif, TIFFCurrentDirectory(in_tif));

    // Close the input TIFF
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}