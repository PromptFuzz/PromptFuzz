#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1217
//<Prompt> ["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFErrorExtR","TIFFFileName","TIFFGetSeekProc","TIFFGetField","TIFFSwabLong","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFNumberOfStrips","TIFFStripSize","_TIFFmalloc","TIFFUnlinkDirectory","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    tdir_t TIFFNumberOfDirectories(TIFF * ),
    int TIFFSetDirectory(TIFF * , tdir_t ),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    const char *TIFFFileName(TIFF * ),
    TIFFSeekProc TIFFGetSeekProc(TIFF * ),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    void TIFFSwabLong(uint32_t * ),
    void *_TIFFmalloc(tmsize_t s),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    uint32_t TIFFNumberOfStrips(TIFF * ),
    tmsize_t TIFFStripSize(TIFF * tif),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFUnlinkDirectory(TIFF * , tdir_t ),
    void TIFFClose(TIFF * tif)
*/
//<score> 21, nr_unique_branch: 9
//<Quality> {"density":14,"unique_branches":{"TIFFSetDirectory":[[2045,9,2045,60,0,0,4,1],[2062,30,2062,55,0,0,4,0],[2063,30,2063,52,0,0,4,0],[2064,30,2064,67,0,0,4,0],[2066,13,2066,21,0,0,4,0],[2080,24,2080,29,0,0,4,0],[2080,33,2080,48,0,0,4,0],[2081,17,2081,75,0,0,4,0]],"_TIFFGetOffsetFromDirNumber":[[5648,9,5648,19,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFFileName","TIFFGetSeekProc","TIFFGetField","TIFFSwabLong","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFNumberOfStrips","TIFFStripSize","TIFFUnlinkDirectory","TIFFClose","_TIFFfree"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFFileName","TIFFGetSeekProc","TIFFGetField","TIFFSwabLong","_TIFFmalloc","TIFFRasterScanlineSize64","TIFFNumberOfStrips","TIFFStripSize","TIFFUnlinkDirectory","TIFFClose","_TIFFfree"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* data, size_t size) {
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

    // Open input TIFF in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the number of directories in the input TIFF
    tdir_t numDirectories = TIFFNumberOfDirectories(in_tif);
    TIFFSetDirectory(in_tif, numDirectories - 1); // Set the last directory as the current directory

    // Get the file name of the input TIFF
    const char* fileName = TIFFFileName(in_tif);

    // Get the seek function of the input TIFF
    TIFFSeekProc seekProc = TIFFGetSeekProc(in_tif);

    // Get a specific tag value from the input TIFF
    uint32_t tagValue;
    TIFFGetField(in_tif, TIFFTAG_COMPRESSION, &tagValue);

    // Perform byte swapping on a long value
    TIFFSwabLong(&tagValue);

    // Allocate memory
    void* memory = _TIFFmalloc(TIFFRasterScanlineSize64(in_tif));

    // Get the number of strips in the input TIFF
    uint32_t numStrips = TIFFNumberOfStrips(in_tif);

    // Get the strip size of the input TIFF
    tmsize_t stripSize = TIFFStripSize(in_tif);

    // Unlink a directory from the input TIFF
    TIFFUnlinkDirectory(in_tif, numDirectories - 1);

    // Close the input TIFF and release allocated resources
    TIFFClose(in_tif);

    // Release allocated memory
    _TIFFfree(memory);

    assert_file_closed(&in_file);
	return 0;
}