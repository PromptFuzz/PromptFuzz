#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 571
//<Prompt> ["TIFFGetMapFileProc","TIFFReadRGBAImageOriented","TIFFErrorExt","TIFFReadRGBATile","TIFFWarningExt","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFWriteDirectory","TIFFReadFromUserBuffer"]
/*<Combination>: [TIFFMapFileProc TIFFGetMapFileProc(TIFF * ),
    int TIFFReadRGBAImageOriented(TIFF * , uint32_t , uint32_t , uint32_t * , int , int ),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFReadRGBATile(TIFF * , uint32_t , uint32_t , uint32_t * ),
    void TIFFWarningExt(thandle_t , const char * , const char * ),
    void TIFFOpenOptionsSetMaxSingleMemAlloc(TIFFOpenOptions * opts, tmsize_t max_single_mem_alloc),
    int TIFFWriteDirectory(TIFF * ),
    int TIFFReadFromUserBuffer(TIFF * tif, uint32_t strile, void * inbuf, tmsize_t insize, void * outbuf, tmsize_t outsize)
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFReadRGBAImageOriented","TIFFClose","TIFFOpen","TIFFClose","TIFFWriteEncodedStrip","TIFFStripSize","TIFFClose","TIFFClose","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFReadRGBAImageOriented","TIFFOpen","TIFFWriteEncodedStrip","TIFFStripSize","TIFFClose","TIFFClose"],"visited":0}
/*Here is the completed code for the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size) {
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

    // Initialize variables
    uint32_t width, height;
    uint32_t* raster;

    // Read RGBA image
    if (TIFFReadRGBAImageOriented(in_tif, width, height, raster, 0, 1) == 0) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output tiff
    TIFF* out_tif = TIFFOpen("output_file", "w");
    if (!out_tif) {
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Write the raster data to the output tiff
    if (TIFFWriteEncodedStrip(out_tif, 0, raster, TIFFStripSize(in_tif)) == -1) {
        TIFFClose(out_tif);
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
    }

    // Cleanup
    TIFFClose(out_tif);
    TIFFClose(in_tif);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}