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
//<ID> 1498
//<Prompt> ["TIFFStreamOpen","TIFFReadBufferSetup","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImageOriented","TIFFStripSize64","TIFFIsByteSwapped","TIFFGetStrileByteCountWithErr","TIFFSetTagExtender","TIFFFlushData","LogL16toY","TIFFReadRGBAStripExt","LogLuv24fromXYZ","TIFFReadRGBATileExt","_TIFFfree","TIFFClose"]
/*<Combination>: [TIFF *TIFFStreamOpen(const char * , std::ostream * ),
    int TIFFReadBufferSetup(TIFF * tif, void * bp, tmsize_t size),
    int TIFFGetField(TIFF * tif, uint32_t tag),
    void *_TIFFmalloc(tmsize_t s),
    int TIFFReadRGBAImageOriented(TIFF * , uint32_t , uint32_t , uint32_t * , int , int ),
    uint64_t TIFFStripSize64(TIFF * tif),
    int TIFFIsByteSwapped(TIFF * ),
    uint64_t TIFFGetStrileByteCountWithErr(TIFF * tif, uint32_t strile, int * pbErr),
    TIFFExtendProc TIFFSetTagExtender(TIFFExtendProc ),
    int TIFFFlushData(TIFF * tif),
    double LogL16toY(int ),
    int TIFFReadRGBAStripExt(TIFF * , uint32_t , uint32_t * , int stop_on_error),
    uint32_t LogLuv24fromXYZ(float * , int ),
    int TIFFReadRGBATileExt(TIFF * , uint32_t , uint32_t , uint32_t * , int stop_on_error),
    void _TIFFfree(void * p),
    void TIFFClose(TIFF * tif)
*/
//<score> 117.8, nr_unique_branch: 78
//<Quality> {"density":31,"unique_branches":{"ZIPCleanup":[[538,9,538,39,0,0,4,0]],"ZIPPostEncode":[[504,13,504,30,0,0,4,0],[505,13,505,22,0,0,4,1],[506,21,506,75,0,0,4,0],[510,25,510,45,0,0,4,1],[514,25,514,70,0,0,4,0],[519,13,519,20,0,0,4,1],[523,14,523,35,0,0,4,1]],"TIFFSetupStrips":[[551,30,551,64,3,0,4,1],[209,34,209,71,10,0,4,0]],"TIFFWriteDirectorySec":[[534,9,534,61,0,0,4,0],[572,21,574,80,0,0,4,1],[576,21,578,80,0,0,4,1],[636,21,638,77,0,0,4,1],[650,21,652,54,0,0,4,1],[678,21,680,80,0,0,4,1],[852,25,852,54,0,0,4,0],[853,25,853,58,0,0,4,1],[209,34,209,71,15,0,4,0],[209,34,209,71,33,0,4,0],[209,34,209,71,37,0,4,0],[209,34,209,71,45,0,4,0]],"TIFFWriteDirectoryTagCheckedRational":[[2363,9,2363,18,0,0,4,1],[2368,14,2368,28,0,0,4,1],[2383,9,2383,35,0,0,4,0]],"TIFFFlushData":[[150,9,150,41,0,0,4,0],[153,13,153,41,0,0,4,1]],"ZIPSetupEncode":[[297,9,297,39,0,0,4,1],[304,9,304,43,0,0,4,1],[307,9,307,56,0,0,4,1]],"TIFFWriteDirectoryTagRational":[[1689,9,1689,20,0,0,4,0],[1689,9,1689,20,0,0,4,1]],"TIFFWriteScanline":[[70,9,70,34,0,0,4,1],[85,9,85,53,0,0,4,1],[105,9,105,32,0,0,4,1],[107,9,107,35,0,0,4,0],[107,9,107,35,0,0,4,1],[112,13,112,32,0,0,4,1],[120,13,120,43,0,0,4,1],[123,13,123,39,0,0,4,1],[129,13,129,52,0,0,4,0],[129,13,129,52,0,0,4,1],[131,17,131,46,0,0,4,1],[142,13,142,48,0,0,4,1],[151,9,151,28,0,0,4,1],[36,6,36,43,1,0,4,0],[36,47,36,79,1,0,4,0],[40,7,40,44,2,0,4,0],[40,7,40,44,2,0,4,1],[40,48,40,64,2,0,4,0],[41,6,41,53,2,0,4,0]],"ZIPPreEncode":[[328,9,328,40,0,0,4,1],[339,28,339,73,0,0,4,0]],"putRGBAAseparate8bittile":[[1356,9,1356,15,4,0,4,0]],"_WriteAsType":[[1813,9,1813,40,0,0,4,1],[1817,14,1817,45,0,0,4,1],[1818,14,1818,44,0,0,4,1],[1819,14,1819,54,0,0,4,1],[1820,14,1820,48,0,0,4,0]],"PredictorSetupEncode":[[231,9,231,33,0,0,4,1],[231,37,231,57,0,0,4,1],[234,9,234,27,0,0,4,1],[292,14,292,32,0,0,4,1]],"_TIFFReadEncodedTileAndAllocBuffer":[[1053,19,1053,57,0,0,4,0]],"TIFFWriteBufferSetup":[[695,13,695,28,0,0,4,0],[250,22,250,62,3,0,4,1]],"TIFFFlushData1":[[928,9,928,27,0,0,4,0],[928,31,928,62,0,0,4,0],[930,13,930,25,0,0,4,1],[933,13,935,50,0,0,4,1],[250,22,250,62,4,0,4,1]],"ZIPEncode":[[460,13,460,40,0,0,4,0],[463,13,463,53,0,0,4,1],[468,13,468,38,0,0,4,1],[479,14,479,20,0,0,4,1]],"DoubleToRational":[[2673,9,2673,22,0,0,4,1],[2682,9,2682,29,0,0,4,1],[2689,9,2689,35,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFClose","TIFFReadRGBAImageOriented","_TIFFfree","TIFFClose","TIFFOpen","_TIFFfree","TIFFClose","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFClose","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadRGBAImageOriented","TIFFOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFClose"],"visited":4}
/*Here is the complete implementation of the LLVMFuzzerTestOneInput_62 function:

*/

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<81) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_19, fdp);
	//fuzzer shim end}




  // write data into input_file
  FILE *in_file = fopen("input_file", "wb");
  if (in_file == NULL) {
    assert_file_closed(&in_file);
	return 0;
  }
  fwrite(data, sizeof(uint8_t), size, in_file);
  assert_file_closed(&in_file);;

  // open input tiff in memory
  std::istringstream s(std::string(data, data + size));
  TIFF *in_tif = TIFFStreamOpen(fuzz_str_1, &s);
  if (!in_tif) {
    assert_file_closed(&in_file);
	return 0;
  }

  // initialize variables
  uint32_t width;
  uint32_t height;
  uint32_t* raster;
  int stop_on_error = 0;
  int error = 0;

  // get width and height from TIFF
  TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);

  // allocate memory for raster
  tmsize_t raster_size = width * height * sizeof(uint32_t);
  raster = (uint32_t*)_TIFFmalloc(raster_size);
  if (!raster) {
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	return 0;
  }

  // read RGBA image from TIFF
  int result = TIFFReadRGBAImageOriented(in_tif, width, height, raster, fuzz_int32_t_2, fuzz_int32_t_3);
  if (!result) {
    _TIFFfree(raster);
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	return 0;
  }

  // perform operations on raster data
  // ...

  // write raster data to output TIFF
  TIFF* out_tif = TIFFOpen("output_file", fuzz_str_4);
  if (!out_tif) {
    _TIFFfree(raster);
    TIFFClose(in_tif);
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
  }

  TIFFSetField(out_tif, fuzz_uint32_t_5, width);
  TIFFSetField(out_tif, fuzz_uint32_t_6, height);
  TIFFSetField(out_tif, fuzz_uint32_t_7, 8);
  TIFFSetField(out_tif, fuzz_uint32_t_8, 4);
  TIFFSetField(out_tif, fuzz_uint32_t_9, PHOTOMETRIC_RGB);
  TIFFSetField(out_tif, fuzz_uint32_t_10, PLANARCONFIG_CONTIG);
  TIFFSetField(out_tif, fuzz_uint32_t_11, TIFFDefaultStripSize(out_tif, fuzz_uint32_t_12));
  TIFFSetField(out_tif, fuzz_uint32_t_13, COMPRESSION_DEFLATE);
  TIFFSetField(out_tif, fuzz_uint32_t_14, ORIENTATION_TOPLEFT);
  TIFFSetField(out_tif, fuzz_uint32_t_15, 72.0);
  TIFFSetField(out_tif, fuzz_uint32_t_16, 72.0);
  TIFFSetField(out_tif, fuzz_uint32_t_17, RESUNIT_INCH);

  tmsize_t raster_size_bytes = width * height * sizeof(uint32_t);
  for (uint32_t row = 0; row < height; row++) {
    if (TIFFWriteScanline(out_tif, &raster[row * width], fuzz_uint32_t_18, fuzz_uint16_t_19) < 0) {
      error = 1;
      break;
    }
  }

  if (error) {
    _TIFFfree(raster);
    TIFFClose(in_tif);
    TIFFClose(out_tif);
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
  }

  _TIFFfree(raster);
  TIFFClose(in_tif);
  TIFFClose(out_tif);

  assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}