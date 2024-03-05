#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 426
//<Prompt> ["TIFFFieldDataType","TIFFRasterScanlineSize64","TIFFRGBAImageEnd","TIFFErrorExtR","TIFFGetCloseProc"]
/*<Combination>: [TIFFDataType TIFFFieldDataType(const TIFFField * ),
    uint64_t TIFFRasterScanlineSize64(TIFF * tif),
    void TIFFRGBAImageEnd(TIFFRGBAImage * ),
    void TIFFErrorExtR(TIFF * , const char * , const char * ),
    TIFFCloseProc TIFFGetCloseProc(TIFF * )
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFFindField","TIFFFieldDataType","TIFFRasterScanlineSize64","TIFFRGBAImageEnd","TIFFErrorExtR","TIFFFileName","TIFFGetCloseProc","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFFindField","TIFFFieldDataType","TIFFRasterScanlineSize64","TIFFRGBAImageEnd","TIFFErrorExtR","TIFFFileName","TIFFGetCloseProc","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

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
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the data type of a TIFF field
    const TIFFField* field = TIFFFindField(in_tif, TIFFTAG_IMAGEWIDTH, TIFF_ANY);
    if (field) {
        TIFFDataType fieldType = TIFFFieldDataType(field);
    }
    
    // Get the raster scanline size for a TIFF image
    uint64_t rasterScanlineSize = TIFFRasterScanlineSize64(in_tif);
    
    // End the RGBA image processing
    TIFFRGBAImage img;
    TIFFRGBAImageEnd(&img);
    
    // Print an error message
    TIFFErrorExtR(in_tif, TIFFFileName(in_tif), "An error occurred");
    
    // Get the closing procedure for a TIFF file
    TIFFCloseProc closeProc = TIFFGetCloseProc(in_tif);

    // Release all allocated resources
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}