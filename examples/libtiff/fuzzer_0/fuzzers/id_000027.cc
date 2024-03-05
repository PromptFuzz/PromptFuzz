#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 467
//<Prompt> ["TIFFGetClientInfo","TIFFSetErrorHandler","TIFFFieldIsAnonymous","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode"]
/*<Combination>: [void *TIFFGetClientInfo(TIFF * , const char * ),
    TIFFErrorHandler TIFFSetErrorHandler(TIFFErrorHandler ),
    int TIFFFieldIsAnonymous(const TIFFField * ),
    TIFFOpenOptions *TIFFOpenOptionsAlloc(),
    void TIFFDefaultTileSize(TIFF * , uint32_t * , uint32_t * ),
    int TIFFSetSubDirectory(TIFF * , uint64_t ),
    uint32_t TIFFDefaultStripSize(TIFF * tif, uint32_t request),
    int uv_encode(double , double , int )
*/
//<score> 10.5, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"TIFFSetSubDirectory":[[2126,9,2126,20,0,0,4,0],[2147,20,2147,31,0,0,4,1]],"uv_encode":[[963,9,963,22,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFGetClientInfo","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","TIFFGetClientInfo","TIFFSetErrorHandler","TIFFFieldWithTag","TIFFOpenOptionsAlloc","TIFFDefaultTileSize","TIFFSetSubDirectory","TIFFDefaultStripSize","uv_encode","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/*Here is the completed code for the fuzz driver using libtiff:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* data, size_t size) {
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

    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Define and initialize variables
    void* client_info = NULL;
    TIFFErrorHandler error_handler = NULL;
    const TIFFField* field = NULL;
    TIFFOpenOptions* open_options = NULL;
    uint32_t default_tile_width = 0, default_tile_height = 0;
    uint32_t default_strip_size = 0;
    int encode_result = 0;

    // Call libtiff APIs
    client_info = TIFFGetClientInfo(in_tif, "client_info");
    error_handler = TIFFSetErrorHandler(NULL);
    field = TIFFFieldWithTag(in_tif, TIFFTAG_IMAGEWIDTH);
    open_options = TIFFOpenOptionsAlloc();
    TIFFDefaultTileSize(in_tif, &default_tile_width, &default_tile_height);
    TIFFSetSubDirectory(in_tif, 0);
    default_strip_size = TIFFDefaultStripSize(in_tif, 0);
    encode_result = uv_encode(0.0, 0.0, 0);

    // Release resources
    TIFFClose(in_tif);
    TIFFOpenOptionsFree(open_options);

    assert_file_closed(&in_file);
	return 0;
}