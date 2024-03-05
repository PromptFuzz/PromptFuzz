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
//<ID> 367
//<Prompt> ["LogLuv24toXYZ","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldSetGetCountSize","TIFFSetMode","TIFFGetMode"]
/*<Combination>: [void LogLuv24toXYZ(uint32_t , float * ),
    void TIFFSwabArrayOfFloat(float * fp, tmsize_t n),
    int TIFFDeferStrileArrayWriting(TIFF * ),
    int TIFFIsUpSampled(TIFF * ),
    int TIFFFieldSetGetCountSize(const TIFFField * ),
    int TIFFSetMode(TIFF * , int ),
    int TIFFGetMode(TIFF * )
*/
//<score> 20, nr_unique_branch: 11
//<Quality> {"density":8,"unique_branches":{"LogLuv24toXYZ":[[1021,9,1021,16,0,0,4,1],[1028,9,1028,34,0,0,4,1]],"uv_decode":[[986,9,986,14,0,0,4,1],[986,18,986,31,0,0,4,1],[990,12,990,29,0,0,4,0],[990,12,990,29,0,0,4,1],[994,13,994,19,0,0,4,0],[994,13,994,19,0,0,4,1],[996,18,996,24,0,0,4,0]],"LogL10toY":[[870,9,870,17,0,0,4,1]],"TIFFFieldSetGetCountSize":[[758,9,758,20,0,0,4,0]]},"library_calls":["TIFFStreamOpen","LogLuv24toXYZ","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldWithName","TIFFFieldSetGetCountSize","TIFFGetMode","TIFFSetMode","TIFFClose"],"critical_calls":["TIFFStreamOpen","LogLuv24toXYZ","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldWithName","TIFFFieldSetGetCountSize","TIFFGetMode","TIFFSetMode","TIFFClose"],"visited":1}
/**/
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




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

    // Event: Convert LogLuv24 to XYZ using libtiff APIs
    uint32_t pixel = 0x12345678;
    float xyz[3];
    LogLuv24toXYZ(fuzz_uint32_t_2, xyz);

    // Call other libtiff APIs
    float float_array[10];
    TIFFSwabArrayOfFloat(float_array, 10);
    TIFFDeferStrileArrayWriting(in_tif);
    int is_upsampled = TIFFIsUpSampled(in_tif);
    const TIFFField* field = TIFFFieldWithName(in_tif, fuzz_str_3);
    int count_size = TIFFFieldSetGetCountSize(field);
    int current_mode = TIFFGetMode(in_tif);
    TIFFSetMode(in_tif, fuzz_int32_t_4);

    // Cleanup resources
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}