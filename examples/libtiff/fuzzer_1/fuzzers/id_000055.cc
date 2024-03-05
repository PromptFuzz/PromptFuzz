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
//<ID> 1124
//<Prompt> ["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldSetGetSize","TIFFFieldWithName","TIFFOpenExt","TIFFSetMode","TIFFGetMode","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    uint64_t TIFFVStripSize64(TIFF * tif, uint32_t nrows),
    void TIFFSwabArrayOfFloat(float * fp, tmsize_t n),
    int TIFFDeferStrileArrayWriting(TIFF * ),
    int TIFFIsUpSampled(TIFF * ),
    void TIFFWarningExt(thandle_t , const char * , const char * ),
    int TIFFFieldSetGetSize(const TIFFField * ),
    const TIFFField *TIFFFieldWithName(TIFF * , const char * ),
    TIFF *TIFFOpenExt(const char * filename, const char * mode, TIFFOpenOptions * opts),
    int TIFFSetMode(TIFF * , int ),
    int TIFFGetMode(TIFF * ),
    void TIFFClose(TIFF * tif)
*/
//<score> 8, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"TIFFClientOpenExt":[[223,9,223,13,0,0,4,0],[223,17,223,47,0,0,4,1],[257,9,257,13,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFGetMode","TIFFSetMode","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFStreamOpen","TIFFVStripSize64","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFWarningExt","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFOpenOptionsAlloc","TIFFOpenExt","TIFFGetMode","TIFFSetMode","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Call the libtiff APIs
    uint64_t stripSize = TIFFVStripSize64(in_tif, fuzz_uint32_t_2);
    float* floatArray = new float[stripSize];
    TIFFSwabArrayOfFloat(floatArray, stripSize);
    int deferResult = TIFFDeferStrileArrayWriting(in_tif);
    int isUpSampled = TIFFIsUpSampled(in_tif);
    TIFFWarningExt(NULL, fuzz_str_3, "Warning message");
    const TIFFField* field = TIFFFieldWithName(in_tif, fuzz_str_4);
    int fieldSize = TIFFFieldSetGetSize(field);
    TIFFOpenOptions* options = TIFFOpenOptionsAlloc();
    TIFF* out_tif = TIFFOpenExt("output_file", "w", options);
    int tiffMode = TIFFGetMode(out_tif);
    TIFFSetMode(out_tif, fuzz_int32_t_5);
    TIFFClose(out_tif);

    // Clean up resources
    TIFFClose(in_tif);
    delete[] floatArray;
    TIFFOpenOptionsFree(options);

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}