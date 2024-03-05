#include "FDSan.h"
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
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

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
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
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
    double converted_val = LogL10toY(log_val);
    
    // Print the directory information to the output file
    FILE* out_file = fopen(filename, "wb");
    if (out_file == NULL) {
        TIFFClose(in_tif);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    TIFFPrintDirectory(in_tif, out_file, 0);
    assert_file_closed(&out_file);;

    // Write the directory to the TIFF file
    TIFFWriteDirectory(in_tif);

    // Convert LogLuv32 to XYZ
    uint32_t logLuv32 = 12345;
    float xyz[3];
    LogLuv32toXYZ(logLuv32, xyz);

    // Set the write offset
    TIFFSetWriteOffset(in_tif, 0);

    // Convert LogLuv24 to XYZ
    uint32_t logLuv24 = 67890;
    float xyz2[3];
    LogLuv24toXYZ(logLuv24, xyz2);
    
    // Close the TIFF file and unmap the memory
    TIFFClose(in_tif);
    unmap_proc(NULL, NULL, 0);
    
    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}