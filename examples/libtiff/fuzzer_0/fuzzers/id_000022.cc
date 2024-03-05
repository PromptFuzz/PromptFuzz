#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 362
//<Prompt> ["LogLuv24toXYZ","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldSetGetCountSize","TIFFSetMode","TIFFGetMode"]
/*<Combination>: [void LogLuv24toXYZ(uint32_t , float * ),
    void TIFFSwabArrayOfFloat(float * fp, tmsize_t n),
    int TIFFDeferStrileArrayWriting(TIFF * ),
    int TIFFIsUpSampled(TIFF * ),
    int TIFFFieldSetGetCountSize(const TIFFField * ),
    int TIFFSetMode(TIFF * , int ),
    int TIFFGetMode(TIFF * )
*/
//<score> 32, nr_unique_branch: 14
//<Quality> {"density":8,"unique_branches":{"TIFFFieldWithName":[[858,9,858,13,0,0,4,0]],"TIFFDeferStrileArrayWriting":[[268,9,268,34,0,0,4,0]],"tagNameCompare":[[565,9,565,12,0,0,4,0]],"_TIFFFindFieldByName":[[823,9,823,28,0,0,4,0],[823,9,823,28,0,0,4,1],[829,9,829,25,0,0,4,1],[841,35,841,38,0,0,4,1],[66,21,66,40,1,0,4,1]],"td_lfind":[[500,34,500,47,0,0,4,0],[500,34,500,47,0,0,4,1],[501,13,501,34,0,0,4,1]],"TIFFFieldSetGetSize":[[685,9,685,20,0,0,4,0]],"TIFFSwabArrayOfFloat":[[186,12,186,19,0,0,4,0],[186,12,186,19,0,0,4,1]]},"library_calls":["TIFFStreamOpen","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldSetGetSize","TIFFFieldWithName","TIFFSetMode","TIFFGetMode","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFSwabArrayOfFloat","TIFFDeferStrileArrayWriting","TIFFIsUpSampled","TIFFFieldSetGetSize","TIFFFieldWithName","TIFFSetMode","TIFFGetMode","TIFFClose"],"visited":1}
/*Here is the complete implementation of the LLVMFuzzerTestOneInput_22 function using the libtiff library APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Write data into input_file
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == nullptr) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, sizeof(uint8_t), size, in_file);
    assert_file_closed(&in_file);;

    // Open input TIFF in memory
    std::istringstream s(std::string(data, data + size));
    TIFF* in_tif = TIFFStreamOpen("MemTIFF", &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Call the libtiff APIs to achieve the desired event
    uint32_t value = 0;
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    TIFFSwabArrayOfFloat(xyz, 3);
    TIFFDeferStrileArrayWriting(in_tif);
    TIFFIsUpSampled(in_tif);
    TIFFFieldSetGetSize(TIFFFieldWithName(in_tif, "FieldName"));
    TIFFSetMode(in_tif, TIFFGetMode(in_tif));

    // Release resources
    TIFFClose(in_tif);

    assert_file_closed(&in_file);
	return 0;
}