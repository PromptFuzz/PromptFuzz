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
//<ID> 1777
//<Prompt> ["TIFFStripSize64","LogL10fromY","TIFFSetFileName","TIFFVStripSize","TIFFIsTiled"]
/*<Combination>: [uint64_t TIFFStripSize64(TIFF * tif),
    int LogL10fromY(double , int ),
    const char *TIFFSetFileName(TIFF * , const char * ),
    tmsize_t TIFFVStripSize(TIFF * tif, uint32_t nrows),
    int TIFFIsTiled(TIFF * )
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"LogL10fromY":[[881,9,881,20,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFStripSize64","LogL10fromY","TIFFSetFileName","TIFFVStripSize","TIFFIsTiled","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFStripSize64","LogL10fromY","TIFFSetFileName","TIFFVStripSize","TIFFIsTiled","TIFFClose"],"visited":0}
/*Here is the completed version of the fuzz driver:

*/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get the strip size
    uint64_t stripSize = TIFFStripSize64(in_tif);

    // Convert color space
    int luma = 100;
    int result = LogL10fromY(luma, fuzz_int32_t_2);

    // Set the output file name
    const char* outputFileName = "output_file";
    TIFFSetFileName(in_tif, outputFileName);

    // Get the vertical strip size
    uint32_t numRows = 10;
    tmsize_t vStripSize = TIFFVStripSize(in_tif, fuzz_uint32_t_3);

    // Check if the tiff is tiled
    int isTiled = TIFFIsTiled(in_tif);

    // Release resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}