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
//<ID> 514
//<Prompt> ["TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ"]
/*<Combination>: [const char *TIFFSetFileName(TIFF * , const char * ),
    TIFFUnmapFileProc TIFFGetUnmapFileProc(TIFF * ),
    double LogL10toY(int ),
    void TIFFPrintDirectory(TIFF * , FILE * , long ),
    int TIFFWriteDirectory(TIFF * ),
    void LogLuv32toXYZ(uint32_t , float * ),
    void TIFFSetWriteOffset(TIFF * tif, toff_t off),
    void LogLuv24toXYZ(uint32_t , float * )
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFClose","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    const char* filename = "output_file";

    // Set the output file name
    TIFFSetFileName(in_tif, filename);

    // Get the unmap file procedure
    TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(in_tif);

    // Convert LogL10 to Y
    double log_val = 10;
    double converted_val = LogL10toY(fuzz_int32_t_2);
    
    // Print the directory information to the output file
    FILE* out_file = fopen(filename, "wb");
    if (out_file == NULL) {
        TIFFClose(in_tif);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    TIFFPrintDirectory(in_tif, out_file, fuzz_int64_t_3);
    assert_file_closed(&out_file);;

    // Write the directory to the TIFF file
    TIFFWriteDirectory(in_tif);

    // Convert LogLuv32 to XYZ
    uint32_t logLuv32 = 12345;
    float xyz[3];
    LogLuv32toXYZ(fuzz_uint32_t_4, xyz);

    // Set the write offset
    TIFFSetWriteOffset(in_tif, fuzz_uint64_t_5);

    // Convert LogLuv24 to XYZ
    uint32_t logLuv24 = 67890;
    float xyz2[3];
    LogLuv24toXYZ(fuzz_uint32_t_6, xyz2);
    
    // Close the TIFF file and unmap the memory
    TIFFClose(in_tif);
    unmap_proc(NULL, NULL, 0);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}