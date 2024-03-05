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
//<ID> 1857
//<Prompt> ["TIFFGetSizeProc","TIFFMergeFieldInfo","TIFFRawStripSize","TIFFRGBAImageOK","TIFFReadGPSDirectory","LogLuv24toXYZ","TIFFVTileSize64"]
/*<Combination>: [TIFFSizeProc TIFFGetSizeProc(TIFF * ),
    int TIFFMergeFieldInfo(TIFF * , const TIFFFieldInfo * , uint32_t ),
    tmsize_t TIFFRawStripSize(TIFF * tif, uint32_t strip),
    int TIFFRGBAImageOK(TIFF * , char [1024]),
    int TIFFReadGPSDirectory(TIFF * tif, toff_t diroff),
    void LogLuv24toXYZ(uint32_t , float * ),
    uint64_t TIFFVTileSize64(TIFF * tif, uint32_t nrows)
*/
//<score> 24, nr_unique_branch: 5
//<Quality> {"density":8,"unique_branches":{"TIFFReadCustomDirectory":[[5122,9,5122,18,0,0,4,0]],"TIFFCleanup":[[107,17,107,56,0,0,4,1]],"TIFFFetchDirectory":[[5757,9,5757,19,0,0,4,1]],"TIFFVGetField":[[1602,13,1602,16,0,0,4,1]],"TIFFMergeFieldInfo":[[1190,9,1190,47,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFGetSizeProc","TIFFMergeFieldInfo","TIFFRawStripSize","TIFFRGBAImageOK","TIFFReadGPSDirectory","LogLuv24toXYZ","TIFFVTileSize64","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetSizeProc","TIFFMergeFieldInfo","TIFFRawStripSize","TIFFRGBAImageOK","TIFFReadGPSDirectory","LogLuv24toXYZ","TIFFVTileSize64","TIFFClose"],"visited":1}
/*Here is the completed LLVMFuzzerTestOneInput_67 function:

*/

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}
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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Call the libtiff APIs
    TIFFSizeProc sizeProc = TIFFGetSizeProc(in_tif);
    int mergeFieldInfoResult = TIFFMergeFieldInfo(in_tif, nullptr, 0);
    uint32_t strip = 0;
    tmsize_t rawStripSize = TIFFRawStripSize(in_tif, fuzz_uint32_t_2);
    char errorMessage[1024];
    int rgbaImageOK = TIFFRGBAImageOK(in_tif, errorMessage);
    toff_t gpsDirectoryOffset = 0;
    int readGPSDirectoryResult = TIFFReadGPSDirectory(in_tif, fuzz_uint64_t_3);
    float xyzValues[3];
    LogLuv24toXYZ(fuzz_uint32_t_4, xyzValues);
    uint32_t nrows = 0;
    uint64_t vTileSize64 = TIFFVTileSize64(in_tif, fuzz_uint32_t_5);

    // Cleanup
    TIFFClose(in_tif);
    remove("input_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}