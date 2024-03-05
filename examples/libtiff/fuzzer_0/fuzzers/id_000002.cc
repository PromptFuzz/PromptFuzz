#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 17
//<Prompt> ["uv_encode","TIFFIsBigEndian","_TIFFmemcpy","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount"]
/*<Combination>: [int uv_encode(double , double , int ),
    int TIFFIsBigEndian(TIFF * ),
    void _TIFFmemcpy(void * d, const void * s, tmsize_t c),
    void TIFFOpenOptionsSetWarningHandlerExtR(TIFFOpenOptions * opts, TIFFErrorHandlerExtR handler, void * warnhandler_user_data),
    int TIFFIsMSB2LSB(TIFF * ),
    int TIFFGetTagListCount(TIFF * )
*/
//<score> 20, nr_unique_branch: 33
//<Quality> {"density":5,"unique_branches":{"tiff_itrunc":[[775,9,775,35,0,0,4,1]],"oog_encode":[[899,9,899,21,0,0,4,0],[903,27,903,30,0,0,4,0],[903,27,903,30,0,0,4,1],[905,27,905,31,0,0,4,0],[905,27,905,31,0,0,4,1],[909,17,909,33,0,0,4,0],[909,17,909,33,0,0,4,1],[909,37,909,44,0,0,4,0],[909,37,909,44,0,0,4,1],[909,48,909,58,0,0,4,1],[911,43,911,50,0,0,4,0],[911,43,911,50,0,0,4,1],[917,21,917,34,0,0,4,0],[917,21,917,34,0,0,4,1],[924,27,924,30,0,0,4,0],[924,27,924,30,0,0,4,1],[925,17,925,29,0,0,4,0],[925,17,925,29,0,0,4,1],[928,30,928,46,0,0,4,0],[929,25,929,54,0,0,4,0],[929,25,929,54,0,0,4,1],[931,30,931,46,0,0,4,0],[932,25,932,64,0,0,4,0],[932,25,932,64,0,0,4,1],[934,21,934,28,0,0,4,0],[934,21,934,28,0,0,4,1]],"uv_encode":[[957,9,957,15,0,0,4,1],[957,19,957,25,0,0,4,1],[963,9,963,22,0,0,4,1],[966,9,966,21,0,0,4,1],[968,9,968,30,0,0,4,1],[971,9,971,29,0,0,4,0]]},"library_calls":["TIFFStreamOpen","uv_encode","TIFFIsBigEndian","_TIFFmemcpy","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","uv_encode","TIFFIsBigEndian","_TIFFmemcpy","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFIsMSB2LSB","TIFFGetTagListCount","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here is the completed code for the fuzz driver:

*/


// Event: Encode the given UV coordinates into an integer value

// Libtiff APIs to achieve the event
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Write data into input_file
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode UV values
    double u = 0.5;
    double v = 0.3;
    int precision = 100;
    int encoded_value = uv_encode(u, v, precision);

    // Check if tiff is big endian
    int is_big_endian = TIFFIsBigEndian(in_tif);

    // Copy memory from source to destination
    void *dest = malloc(size);
    if (dest != NULL) {
        _TIFFmemcpy(dest, data, size);
    }

    // Set warning handler
    TIFFOpenOptions *options = TIFFOpenOptionsAlloc();
    TIFFOpenOptionsSetWarningHandlerExtR(options, nullptr, nullptr);

    // Check if tiff is MSB2LSB
    int is_msb2lsb = TIFFIsMSB2LSB(in_tif);

    // Get tag list count
    int tag_list_count = TIFFGetTagListCount(in_tif);

    // Release allocated resources
    TIFFClose(in_tif);
    free(dest);
    TIFFOpenOptionsFree(options);

    assert_file_closed(&in_file);
	return 0;
}