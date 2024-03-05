#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 554
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
//<Quality> {"density":9,"unique_branches":{"TIFFErrorExtR":[[119,9,119,26,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFClientdata","TIFFGetClientInfo","TIFFIsMSB2LSB","TIFFIsCODECConfigured","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","TIFFClientdata","TIFFGetClientInfo","TIFFIsMSB2LSB","TIFFIsCODECConfigured","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here is the completed code for the fuzz driver using the libtiff library:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get client data
    thandle_t client_data = TIFFClientdata(in_tif);

    // Get client info
    void* client_info = TIFFGetClientInfo(in_tif, fuzz_str_2);

    // Check if TIFF is MSB2LSB
    int is_msb2lsb = TIFFIsMSB2LSB(in_tif);

    // Check if CODEC is configured
    int is_codec_configured = TIFFIsCODECConfigured(fuzz_uint16_t_3);

    // Set error handler
    TIFFSetErrorHandler(NULL);

    // Get TIFF field with tag
    const TIFFField* tiff_field = TIFFFieldWithTag(in_tif, fuzz_uint32_t_4);

    // Allocate TIFFOpenOptions
    TIFFOpenOptions* tiff_options = TIFFOpenOptionsAlloc();

    // Set default tile size
    uint32_t tile_width, tile_height;
    TIFFDefaultTileSize(in_tif, &tile_width, &tile_height);

    // Set sub-directory
    int sub_dir = TIFFSetSubDirectory(in_tif, fuzz_uint64_t_5);

    // Get default strip size
    uint32_t default_strip_size = TIFFDefaultStripSize(in_tif, fuzz_uint32_t_6);

    // Encode UV value
    int uv_encoded = uv_encode(1.234, 5.678, fuzz_int32_t_7);

    // Close TIFF file
    TIFFClose(in_tif);

    // Free TIFFOpenOptions
    TIFFOpenOptionsFree(tiff_options);

    assert_file_closed(&in_file);
	return 0;
}