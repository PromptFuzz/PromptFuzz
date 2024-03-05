#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 1159
//<Prompt> ["TIFFUnRegisterCODEC","TIFFOpenOptionsSetWarningHandlerExtR","TIFFWriteTile","TIFFFileno","TIFFCurrentDirOffset","XYZtoRGB24","TIFFIsBigTIFF"]
/*<Combination>: [void TIFFUnRegisterCODEC(TIFFCodec * ),
    void TIFFOpenOptionsSetWarningHandlerExtR(TIFFOpenOptions * opts, TIFFErrorHandlerExtR handler, void * warnhandler_user_data),
    tmsize_t TIFFWriteTile(TIFF * tif, void * buf, uint32_t x, uint32_t y, uint32_t z, uint16_t s),
    int TIFFFileno(TIFF * ),
    uint64_t TIFFCurrentDirOffset(TIFF * ),
    void XYZtoRGB24(float * , uint8_t * ),
    int TIFFIsBigTIFF(TIFF * )
*/
//<score> 15, nr_unique_branch: 8
//<Quality> {"density":5,"unique_branches":{"TIFFUnRegisterCODEC":[[237,35,237,54,0,0,4,0],[238,13,238,26,0,0,4,0]],"XYZtoRGB24":[[859,24,859,33,0,0,4,1],[859,40,859,49,0,0,4,0],[860,24,860,33,0,0,4,1],[860,40,860,49,0,0,4,0],[861,24,861,33,0,0,4,1],[861,40,861,49,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFRegisterCODEC","TIFFUnRegisterCODEC","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFCurrentDirOffset","TIFFIsBigTIFF","XYZtoRGB24","TIFFFileno","TIFFOpen","TIFFWriteTile","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","TIFFRegisterCODEC","TIFFUnRegisterCODEC","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFCurrentDirOffset","TIFFIsBigTIFF","XYZtoRGB24","TIFFFileno","TIFFOpen","TIFFWriteTile","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* data, size_t size) {
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

    TIFFCodec* codec = TIFFRegisterCODEC(0, "", NULL);

    // Unregister the codec
    TIFFUnRegisterCODEC(codec);

    // Create open options and set warning handler
    TIFFOpenOptions* options = TIFFOpenOptionsAlloc();
    TIFFOpenOptionsSetWarningHandlerExtR(options, NULL, NULL);

    // Set current directory offset
    uint64_t offset = TIFFCurrentDirOffset(in_tif);

    // Check if the TIFF file is bigTIFF
    int isBigTIFF = TIFFIsBigTIFF(in_tif);

    // Convert XYZ to RGB24
    float xyz[3] = {1.0f, 1.0f, 1.0f};
    uint8_t rgb[3];
    XYZtoRGB24(xyz, rgb);

    // Get the file descriptor of the input TIFF file
    int in_fd = TIFFFileno(in_tif);

    // Write a tile to the output TIFF file
    TIFF* out_tif = TIFFOpen("output_file", "w");
    if (out_tif) {
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        tmsize_t result = TIFFWriteTile(out_tif, NULL, x, y, z, s);
        TIFFClose(out_tif);
    }

    // Release resources
    TIFFClose(in_tif);
    TIFFOpenOptionsFree(options);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}