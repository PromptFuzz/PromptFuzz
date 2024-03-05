#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 79
//<Prompt> ["TIFFTileSize64","TIFFErrorExt","TIFFForceStrileArrayWriting","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFFileName"]
/*<Combination>: [uint64_t TIFFTileSize64(TIFF * tif),
    void TIFFErrorExt(thandle_t , const char * , const char * ),
    int TIFFForceStrileArrayWriting(TIFF * ),
    TIFFTagMethods *TIFFAccessTagMethods(TIFF * ),
    uint32_t LogLuv32fromXYZ(float * , int ),
    const char *TIFFFileName(TIFF * )
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"tiff_itrunc":[[775,9,775,35,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFFileName","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFForceStrileArrayWriting","TIFFErrorExt","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFTileSize64","TIFFFileName","TIFFAccessTagMethods","LogLuv32fromXYZ","TIFFForceStrileArrayWriting","TIFFErrorExt","TIFFClose"],"visited":0}
/*Here is the complete C++ program that uses the libtiff library to achieve the specified event:

*/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* data, size_t size) {
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

    // Get the tile size using TIFFTileSize64 API
    uint64_t tile_size = TIFFTileSize64(in_tif);
    printf("Tile size: %lu\n", tile_size);

    // Get the file name using TIFFFileName API
    const char* filename = TIFFFileName(in_tif);
    printf("File name: %s\n", filename);

    // Access tag methods using TIFFAccessTagMethods API
    TIFFTagMethods* tag_methods = TIFFAccessTagMethods(in_tif);
    if (tag_methods) {
        printf("Tag methods accessed\n");
    }

    // Convert XYZ to LogLuv32 using LogLuv32fromXYZ API
    float xyz[3] = { 0.5, 0.2, 0.3 };
    uint32_t logluv = LogLuv32fromXYZ(xyz, 0);
    printf("LogLuv32 value: %u\n", logluv);

    // Force strile array writing using TIFFForceStrileArrayWriting API
    int force_strile = TIFFForceStrileArrayWriting(in_tif);
    if (force_strile) {
        printf("Strile array writing forced\n");
    }

    // Handle TIFF error using TIFFErrorExt API
    TIFFErrorExt((thandle_t)in_file, "Error", "Custom error message");

    // Close the input TIFF and release resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}