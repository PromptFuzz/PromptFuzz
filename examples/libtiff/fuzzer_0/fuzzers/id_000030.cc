#include "FDSan.h"
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
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
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

    // Open input TIFF in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
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
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    TIFFPrintDirectory(in_tif, out_file, 0);
    assert_file_closed(&out_file);;

    // Write the current directory to the output file
    TIFFWriteDirectory(in_tif);

    // Convert LogLuv32 to XYZ
    uint32_t logLuv32 = 123;
    float xyz[3];
    LogLuv32toXYZ(logLuv32, xyz);

    // Set the write offset of the TIFF
    TIFFSetWriteOffset(in_tif, 0);

    // Convert LogLuv24 to XYZ
    uint32_t logLuv24 = 234;
    float xyz2[3];
    LogLuv24toXYZ(logLuv24, xyz2);

    // Close the input TIFF and release resources
    TIFFClose(in_tif);
    unmap_file_proc(NULL, NULL, 0);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}