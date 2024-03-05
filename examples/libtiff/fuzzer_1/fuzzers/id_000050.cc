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
//<ID> 819
//<Prompt> ["TIFFVSetField","TIFFXYZToRGB","TIFFFieldName","TIFFUnsetField","TIFFFieldReadCount","TIFFSwabArrayOfDouble","TIFFIsUpSampled"]
/*<Combination>: [int TIFFVSetField(TIFF * , uint32_t , __va_list_tag * ),
    void TIFFXYZToRGB(TIFFCIELabToRGB * , float , float , float , uint32_t * , uint32_t * , uint32_t * ),
    const char *TIFFFieldName(const TIFFField * ),
    int TIFFUnsetField(TIFF * , uint32_t ),
    int TIFFFieldReadCount(const TIFFField * ),
    void TIFFSwabArrayOfDouble(double * dp, tmsize_t n),
    int TIFFIsUpSampled(TIFF * )
*/
//<score> 44, nr_unique_branch: 24
//<Quality> {"density":11,"unique_branches":{"TIFFFieldSetGetCountSize":[[768,9,768,36,0,0,4,0],[770,9,770,35,0,0,4,0],[776,9,776,35,0,0,4,0],[777,9,777,35,0,0,4,0],[778,9,778,35,0,0,4,0],[779,9,779,36,0,0,4,0],[780,9,780,36,0,0,4,0],[781,9,781,36,0,0,4,0],[782,9,782,36,0,0,4,0],[783,9,783,35,0,0,4,0],[784,9,784,36,0,0,4,0],[785,9,785,36,0,0,4,0],[786,9,786,36,0,0,4,0],[787,9,787,34,0,0,4,0],[789,9,789,16,0,0,4,1]],"TIFFUnsetField":[[1165,9,1165,13,0,0,4,1],[1168,9,1168,39,0,0,4,1],[1175,21,1175,48,0,0,4,0],[1179,17,1179,43,0,0,4,0],[1179,17,1179,43,0,0,4,1],[1183,13,1183,40,0,0,4,0],[1186,20,1186,51,0,0,4,0],[1186,20,1186,51,0,0,4,1]],"TIFFGetTagListEntry":[[47,26,47,62,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFGetTagListCount","TIFFGetTagListEntry","TIFFFieldWithTag","TIFFFieldName","TIFFFieldReadCount","TIFFFieldSetGetCountSize","TIFFUnsetField","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFGetTagListCount","TIFFGetTagListEntry","TIFFFieldWithTag","TIFFFieldName","TIFFFieldReadCount","TIFFUnsetField","TIFFFieldSetGetCountSize","TIFFClose"],"visited":0}
/*Here's the completed code for the LLVMFuzzerTestOneInput_50 function using the libtiff APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




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
    TIFF* in_tif = TIFFStreamOpen(fuzz_str_1, &s);
    if (!in_tif) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the number of directories in the TIFF file
    tdir_t num_dirs = TIFFNumberOfDirectories(in_tif);
    
    // Loop through each directory
    for (tdir_t dir = 0; dir < num_dirs; dir++) {
        // Set the current directory
        if (!TIFFSetDirectory(in_tif, fuzz_uint32_t_2)) {
            continue;
        }
        
        // Get the number of tags in the current directory
        uint32_t num_tags = TIFFGetTagListCount(in_tif);
        
        // Loop through each tag in the directory
        for (uint32_t tag_index = 0; tag_index < num_tags; tag_index++) {
            uint32_t tag = TIFFGetTagListEntry(in_tif, fuzz_int32_t_3);
            const TIFFField* field = TIFFFieldWithTag(in_tif, tag);
            
            if (field != nullptr) {
                const char* field_name = TIFFFieldName(field);
                int field_count = TIFFFieldReadCount(field);
                
                std::cout << "Tag: " << field_name << ", Count: " << field_count << std::endl;
                
                // Check if the tag is unsettable
                if (TIFFFieldSetGetCountSize(field) != 0 && TIFFUnsetField(in_tif, tag) != 0) {
                    std::cout << "Tag unset successfully" << std::endl;
                }
            }
        }
    }
    
    // Cleanup
    TIFFClose(in_tif);
    
    assert_file_closed(&in_file);
	return 0;
}