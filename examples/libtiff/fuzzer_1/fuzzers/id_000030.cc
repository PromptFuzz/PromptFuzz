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
//<ID> 510
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
//<score> 2.6666667, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFClose","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"visited":2}
/**/
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    // Write data into input_file
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input TIFF in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the output TIFF file name
    const char *out_file_name = "output_file";
    TIFFSetFileName(in_tif, out_file_name);

    // Get the unmap file proc
    TIFFUnmapFileProc unmap_file_proc = TIFFGetUnmapFileProc(in_tif);

    // Convert LogL10 to Y
    double logL10 = 10.0;
    int y = LogL10toY(logL10);

    // Print the input directory
    FILE *out_file = fopen(out_file_name, "wb");
    if (out_file == NULL) {
        TIFFClose(in_tif);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }
    TIFFPrintDirectory(in_tif, out_file, fuzz_int64_t_2);
    assert_file_closed(&out_file);;

    // Write the current directory to the output file
    TIFFWriteDirectory(in_tif);

    // Convert LogLuv32 to XYZ
    uint32_t logLuv32 = 123;
    float xyz[3];
    LogLuv32toXYZ(fuzz_uint32_t_3, xyz);

    // Set the write offset of the TIFF
    TIFFSetWriteOffset(in_tif, fuzz_uint64_t_4);

    // Convert LogLuv24 to XYZ
    uint32_t logLuv24 = 234;
    float xyz2[3];
    LogLuv24toXYZ(fuzz_uint32_t_5, xyz2);

    // Close the input TIFF and release resources
    TIFFClose(in_tif);
    unmap_file_proc(NULL, NULL, 0);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}