#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 831
//<Prompt> ["TIFFRasterScanlineSize","TIFFCurrentTile","TIFFWarningExtR","TIFFLastDirectory","TIFFSwabLong8","TIFFWarning","TIFFGetVersion"]
/*<Combination>: [tmsize_t TIFFRasterScanlineSize(TIFF * tif),
    uint32_t TIFFCurrentTile(TIFF * tif),
    void TIFFWarningExtR(TIFF * , const char * , const char * ),
    int TIFFLastDirectory(TIFF * ),
    void TIFFSwabLong8(uint64_t * ),
    void TIFFWarning(const char * , const char * ),
    const char *TIFFGetVersion()
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFRasterScanlineSize","TIFFCurrentTile","TIFFWarningExtR","TIFFLastDirectory","TIFFSwabLong8","TIFFWarning","TIFFGetVersion","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFRasterScanlineSize","TIFFCurrentTile","TIFFWarningExtR","TIFFLastDirectory","TIFFSwabLong8","TIFFWarning","TIFFGetVersion","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* data, size_t size) {
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

    // Call the libtiff APIs
    tmsize_t scanlineSize = TIFFRasterScanlineSize(in_tif);
    uint32_t currentTile = TIFFCurrentTile(in_tif);
    TIFFWarningExtR(in_tif, "warning ext", "warning message");
    int lastDirectory = TIFFLastDirectory(in_tif);

    uint64_t swabLong8Value = 1234567890;
    TIFFSwabLong8(&swabLong8Value);

    TIFFWarning("warning", "warning message");

    const char* version = TIFFGetVersion();

    // Release resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}