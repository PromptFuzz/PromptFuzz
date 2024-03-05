#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 557
//<Prompt> ["TIFFStreamOpen","TIFFGetClientInfo","TIFFClientdata","TIFFIsMSB2LSB","TIFFIsCODECConfigured","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    void *TIFFGetClientInfo(TIFF * , const char * ),
    thandle_t TIFFClientdata(TIFF * ),
    int TIFFIsMSB2LSB(TIFF * ),
    int TIFFIsCODECConfigured(uint16_t ),
    TIFFErrorHandler TIFFSetErrorHandler(TIFFErrorHandler ),
    const TIFFField *TIFFFieldWithTag(TIFF * , uint32_t ),
    TIFFOpenOptions *TIFFOpenOptionsAlloc(),
    void TIFFDefaultTileSize(TIFF * , uint32_t * , uint32_t * ),
    int TIFFSetSubDirectory(TIFF * , uint64_t ),
    uint32_t TIFFDefaultStripSize(TIFF * tif, uint32_t request),
    int uv_encode(double , double , int ),
    void TIFFClose(TIFF * tif),
    void TIFFOpenOptionsFree(TIFFOpenOptions * )
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"_TIFFDefaultStripSize":[[213,9,213,23,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFGetClientInfo","TIFFClientdata","TIFFIsMSB2LSB","TIFFIsCODECConfigured","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","TIFFGetClientInfo","TIFFClientdata","TIFFIsMSB2LSB","TIFFIsCODECConfigured","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size) {
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

    // Get client info
    void* client_info = TIFFGetClientInfo(in_tif, "client_info");

    // Get client data
    thandle_t client_data = TIFFClientdata(in_tif);

    // Check if MSB to LSB
    int is_MSB2LSB = TIFFIsMSB2LSB(in_tif);

    // Check if CODEC is configured
    int is_CODECConfigured = TIFFIsCODECConfigured(0x1234);

    // Set error handler
    TIFFErrorHandler error_handler = [](const char* module, const char* fmt, va_list args) {
        vfprintf(stderr, fmt, args);
    };
    TIFFSetErrorHandler(error_handler);

    // Get TIFF field with tag
    const TIFFField* field = TIFFFieldWithTag(in_tif, 0x5678);

    // Allocate TIFFOpenOptions
    TIFFOpenOptions* options = TIFFOpenOptionsAlloc();

    // Set default tile size
    uint32_t tile_width, tile_height;
    TIFFDefaultTileSize(in_tif, &tile_width, &tile_height);

    // Set subdirectory
    int set_subdirectory_result = TIFFSetSubDirectory(in_tif, 0xabcdef);

    // Get default strip size
    uint32_t default_strip_size = TIFFDefaultStripSize(in_tif, 100);

    // Encode UV values
    int uv_encode_result = uv_encode(0.5, 0.3, 1);

    // Close the input TIFF
    TIFFClose(in_tif);

    // Free TIFFOpenOptions
    TIFFOpenOptionsFree(options);

    assert_file_closed(&in_file);
	return 0;
}