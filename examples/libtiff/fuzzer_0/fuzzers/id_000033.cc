#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 519
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
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFSetFileName","TIFFGetUnmapFileProc","LogL10toY","TIFFPrintDirectory","TIFFWriteDirectory","LogLuv32toXYZ","TIFFSetWriteOffset","LogLuv24toXYZ","TIFFClose"],"visited":0}
/**/
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // write data into input_file.
    FILE *in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // open input tiff in memory
    std::istringstream s(std::string(data, data + size));
    TIFF *in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Set the output file name
    const char* outputFileName = "output_file";
    TIFFSetFileName(in_tif, outputFileName);

    // Get the UnmapFileProc function pointer
    TIFFUnmapFileProc unmapFileProc = TIFFGetUnmapFileProc(in_tif);

    // Convert LogL10 values to Y values
    double logL10Value = 10.0;
    double yValue = LogL10toY(logL10Value);

    // Print the directory of the TIFF file
    FILE* outputFile = fopen(outputFileName, "wb");
    TIFFPrintDirectory(in_tif, outputFile, 0);
    assert_file_closed(&outputFile);;

    // Write the current directory to the TIFF file
    TIFFWriteDirectory(in_tif);

    // Convert LogLuv32 values to XYZ values
    uint32_t logLuv32Value = 32;
    float xyzValues[3];
    LogLuv32toXYZ(logLuv32Value, xyzValues);

    // Set the write offset for the TIFF file
    toff_t writeOffset = 0;
    TIFFSetWriteOffset(in_tif, writeOffset);

    // Convert LogLuv24 values to XYZ values
    uint32_t logLuv24Value = 24;
    float xyzValues2[3];
    LogLuv24toXYZ(logLuv24Value, xyzValues2);

    // Cleanup and close the TIFF file
    TIFFClose(in_tif);

    // Unmap the file if necessary
    if (unmapFileProc) {
        unmapFileProc(in_tif, nullptr, 0);
    }

    // Open output file to read the result
    FILE* outFile = fopen(outputFileName, "rb");
    if (outFile) {
        // read the output file if needed
        assert_file_closed(&outFile);;
    }

    assert_file_name_closed("output_file");
	assert_file_closed(&outFile);
	assert_file_closed(&outputFile);
	assert_file_closed(&in_file);
	return 0;
}