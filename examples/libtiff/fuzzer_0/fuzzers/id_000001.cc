#include "FDSan.h"
#include <tiffio.hxx>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 10
//<Prompt> ["TIFFReadBufferSetup","TIFFDeferStrileArrayWriting","TIFFSetDirectory","TIFFComputeStrip","_TIFFfree","TIFFRawStripSize","TIFFFlush"]
/*<Combination>: [int TIFFReadBufferSetup(TIFF * tif, void * bp, tmsize_t size),
    int TIFFDeferStrileArrayWriting(TIFF * ),
    int TIFFSetDirectory(TIFF * , tdir_t ),
    uint32_t TIFFComputeStrip(TIFF * , uint32_t , uint16_t ),
    void _TIFFfree(void * p),
    tmsize_t TIFFRawStripSize(TIFF * tif, uint32_t strip),
    int TIFFFlush(TIFF * tif)
*/
//<score> 198, nr_unique_branch: 458
//<Quality> {"density":12,"unique_branches":{"OJPEGReadBlock":[[2356,13,2356,36,0,0,4,0],[2358,17,2358,45,0,0,4,0],[2358,17,2358,45,0,0,4,1],[2363,13,2363,35,0,0,4,0],[2370,14,2370,22,0,0,4,0]],"TIFFFlush":[[50,9,50,64,0,0,4,1]],"TIFFCheckRead":[[1494,23,1494,28,0,0,4,1]],"OJPEGPreDecode":[[776,13,776,46,0,0,4,1],[783,17,783,54,0,0,4,0],[783,17,783,54,0,0,4,1],[250,22,250,62,1,0,4,1]],"PixarLogSetupDecode":[[756,9,756,42,0,0,4,0],[777,9,777,25,0,0,4,0]],"ThunderDecodeRow":[[174,9,174,36,0,0,4,0],[174,9,174,36,0,0,4,1],[179,12,179,19,0,0,4,0],[179,12,179,19,0,0,4,1],[181,13,181,65,0,0,4,0],[181,13,181,65,0,0,4,1]],"PixarLogDecode":[[897,13,897,34,0,0,4,1],[904,13,904,26,0,0,4,0],[904,13,904,26,0,0,4,1],[907,27,907,41,0,0,4,1],[910,14,910,38,0,0,4,0],[910,14,910,38,0,0,4,1],[913,9,913,34,0,0,4,1],[927,9,927,35,0,0,4,1],[935,9,935,24,0,0,4,1],[944,17,944,29,0,0,4,1]],"TIFFRawStripSize":[[162,9,162,28,0,0,4,0],[162,9,162,28,0,0,4,1],[167,13,167,29,0,0,4,1]],"PredictorSetup":[[81,9,81,37,0,0,4,0],[82,17,82,59,0,0,4,0],[250,22,250,62,7,0,4,1]],"OJPEGPostDecode":[[996,9,996,66,0,0,4,0]],"TIFFWriteEncodedStrip":[[36,6,36,43,1,0,4,1],[36,47,36,79,1,0,4,1]],"_TIFFGetStrileOffsetOrByteCountValue":[[7791,13,7791,18,0,0,4,1]],"OJPEGReadHeaderInfoSecStreamSos":[[1970,9,1970,25,0,0,4,0],[1970,9,1970,25,0,0,4,1],[1976,9,1976,35,0,0,4,0],[1976,9,1976,35,0,0,4,1],[1978,9,1978,53,0,0,4,0]],"TIFFClientOpenExt":[[347,13,347,21,0,0,4,0],[349,21,349,32,0,0,4,0],[434,13,434,41,0,0,4,0],[440,17,440,43,0,0,4,0],[473,13,473,64,0,0,4,0]],"ZIPSetupDecode":[[116,9,116,46,0,0,4,1]],"Fax3BadLength":[[250,22,250,62,1,0,4,1],[250,22,250,62,2,0,4,1]],"OJPEGReadHeaderInfo":[[1194,13,1194,46,0,0,4,0],[1194,13,1194,46,0,0,4,1],[250,22,250,62,1,0,4,1]],"TIFFReadRawStrip1":[[600,9,600,18,0,0,4,0],[604,13,604,20,0,0,4,0],[604,13,604,20,0,0,4,1],[612,13,612,23,0,0,4,0],[612,13,612,23,0,0,4,1]],"PackBitsDecode":[[277,17,277,40,0,0,4,0]],"OJPEGReadHeaderInfoSecStreamDht":[[1727,13,1727,40,0,0,4,0],[1749,13,1749,70,0,0,4,0],[1755,13,1755,27,0,0,4,1],[1757,17,1757,22,0,0,4,0],[1763,17,1763,36,0,0,4,0],[1769,17,1769,32,0,0,4,0],[1769,17,1769,32,0,0,4,1],[1776,17,1776,22,0,0,4,0],[1776,17,1776,22,0,0,4,1],[1782,17,1782,36,0,0,4,0],[1782,17,1782,36,0,0,4,1]],"OJPEGDecode":[[880,9,880,42,0,0,4,1],[887,13,887,52,0,0,4,0],[887,13,887,52,0,0,4,1]],"ThunderDecode":[[97,12,97,18,0,0,4,0],[97,22,97,41,0,0,4,0],[97,22,97,41,0,0,4,1],[103,17,103,33,0,0,4,1],[105,13,105,29,0,0,4,0],[105,13,105,29,0,0,4,1],[110,21,110,32,0,0,4,0],[110,21,110,32,0,0,4,1],[120,21,120,40,0,0,4,0],[120,21,120,40,0,0,4,1],[122,28,122,33,0,0,4,0],[122,28,122,33,0,0,4,1],[125,21,125,28,0,0,4,0],[125,21,125,28,0,0,4,1],[129,13,129,36,0,0,4,0],[129,13,129,36,0,0,4,1],[130,21,130,60,0,0,4,0],[130,21,130,60,0,0,4,1],[133,21,133,60,0,0,4,0],[133,21,133,60,0,0,4,1],[136,21,136,53,0,0,4,0],[136,21,136,53,0,0,4,1],[140,13,140,36,0,0,4,0],[140,13,140,36,0,0,4,1],[141,21,141,60,0,0,4,0],[141,21,141,60,0,0,4,1],[144,21,144,53,0,0,4,0],[144,21,144,53,0,0,4,1],[148,13,148,29,0,0,4,0],[148,13,148,29,0,0,4,1],[155,9,155,29,0,0,4,0],[155,9,155,29,0,0,4,1],[159,23,159,42,0,0,4,1],[60,13,60,32,5,0,4,0],[62,17,62,30,5,0,4,0],[62,17,62,30,5,0,4,1],[60,13,60,32,7,0,4,0],[60,13,60,32,7,0,4,1],[62,17,62,30,7,0,4,0],[62,17,62,30,7,0,4,1],[60,13,60,32,9,0,4,0],[60,13,60,32,9,0,4,1],[62,17,62,30,9,0,4,0],[62,17,62,30,9,0,4,1],[60,13,60,32,12,0,4,0],[62,17,62,30,12,0,4,0],[62,17,62,30,12,0,4,1],[60,13,60,32,14,0,4,0],[60,13,60,32,14,0,4,1],[62,17,62,30,14,0,4,0],[62,17,62,30,14,0,4,1],[60,13,60,32,16,0,4,0],[62,17,62,30,16,0,4,0],[62,17,62,30,16,0,4,1]],"OJPEGWriteStreamCompressed":[[2612,9,2612,32,0,0,4,1],[2621,9,2621,37,0,0,4,1],[2625,13,2625,29,0,0,4,1],[2626,21,2626,75,0,0,4,1],[2634,13,2634,20,0,0,4,0]],"ZIPDecode":[[264,13,264,34,0,0,4,1],[270,13,270,26,0,0,4,0],[270,13,270,26,0,0,4,1],[275,14,275,21,0,0,4,0],[60,23,60,47,7,0,4,0]],"TIFFFillStrip":[[740,9,740,47,0,0,4,0],[740,9,740,47,0,0,4,1],[743,13,743,27,0,0,4,0],[743,13,743,27,0,0,4,1],[743,31,743,67,0,0,4,1],[755,13,755,36,0,0,4,0],[755,13,755,36,0,0,4,1],[760,17,760,31,0,0,4,0],[760,35,760,80,0,0,4,0],[760,35,760,80,0,0,4,1],[850,17,850,50,0,0,4,1],[855,17,855,50,0,0,4,0],[855,17,855,50,0,0,4,1],[858,21,858,58,0,0,4,1],[866,17,866,49,0,0,4,1],[889,21,890,66,0,0,4,0],[889,21,890,66,0,0,4,1],[899,17,899,29,0,0,4,0],[899,17,899,29,0,0,4,1],[900,17,900,54,0,0,4,0],[900,17,900,54,0,0,4,1],[251,23,251,62,3,0,4,1],[251,23,251,62,5,0,4,1],[251,23,251,62,16,0,4,1]],"OJPEGWriteHeaderInfo":[[1322,9,1322,40,0,0,4,0],[1352,9,1352,46,0,0,4,1],[1431,18,1431,63,0,0,4,1]],"OJPEGReadHeaderInfoSecTablesDcTable":[[2088,26,2088,78,0,0,4,1],[2092,21,2092,67,0,0,4,0],[2101,17,2101,24,0,0,4,0]],"Fax3Decode2D":[[150,21,150,35,4,0,4,0],[395,21,395,31,7,0,4,0],[398,21,398,33,7,0,4,1],[401,21,401,35,7,0,4,0],[402,21,402,34,7,0,4,0],[406,21,406,28,7,0,4,0],[412,17,412,28,7,0,4,0],[495,33,495,47,8,0,4,0],[510,33,510,45,8,0,4,1],[513,33,513,47,8,0,4,0],[514,33,514,46,8,0,4,0],[518,33,518,40,8,0,4,0],[534,33,534,47,8,0,4,0],[552,33,552,47,8,0,4,0],[553,33,553,46,8,0,4,0],[170,21,170,35,15,0,4,0],[117,21,117,31,24,0,4,0],[326,13,326,22,40,0,4,0],[331,20,331,30,40,0,4,0],[331,34,331,46,40,0,4,0],[328,13,328,24,41,0,4,1],[337,21,337,39,41,0,4,0],[432,13,432,26,46,0,4,0],[433,20,433,28,46,0,4,0],[433,20,433,28,46,0,4,1],[433,32,433,42,46,0,4,0],[435,21,435,54,46,0,4,1],[433,20,433,28,70,0,4,1],[432,13,432,26,77,0,4,0],[433,20,433,28,77,0,4,0],[433,20,433,28,77,0,4,1],[433,32,433,42,77,0,4,0],[435,21,435,54,77,0,4,1],[337,21,337,39,96,0,4,0],[333,17,333,27,101,0,4,1],[341,22,341,32,101,0,4,1],[166,13,166,28,119,0,4,1],[170,21,170,35,119,0,4,1],[177,21,177,43,119,0,4,0],[268,13,268,38,124,0,4,1],[268,13,268,38,132,0,4,1],[166,13,166,28,145,0,4,1],[166,13,166,28,150,0,4,0],[170,21,170,35,150,0,4,1],[177,21,177,43,150,0,4,0],[177,21,177,43,150,0,4,1],[166,13,166,28,155,0,4,1],[170,21,170,35,155,0,4,1],[177,21,177,43,155,0,4,0],[170,21,170,35,160,0,4,1],[177,21,177,43,160,0,4,0],[268,13,268,38,186,0,4,1],[117,21,117,31,213,0,4,0],[117,21,117,31,216,0,4,0],[117,21,117,31,217,0,4,0],[117,21,117,31,217,0,4,1],[117,21,117,31,244,0,4,0],[117,21,117,31,247,0,4,0],[117,21,117,31,247,0,4,1],[117,21,117,31,248,0,4,0],[117,21,117,31,248,0,4,1],[117,21,117,31,251,0,4,0],[117,21,117,31,252,0,4,1],[117,21,117,31,255,0,4,0],[117,21,117,31,256,0,4,0],[117,21,117,31,256,0,4,1]],"LogLuvSetupDecode":[[1515,9,1515,32,0,0,4,1],[1551,9,1551,30,0,0,4,1],[1565,9,1565,16,0,0,4,0]],"OJPEGReadHeaderInfoSecTablesQTable":[[2027,26,2027,76,0,0,4,1],[2031,21,2031,65,0,0,4,1],[2052,17,2052,24,0,0,4,0]],"Fax3SetupState":[[250,22,250,62,3,0,4,1]],"LZWDecodeCompat":[[890,22,890,40,0,0,4,0],[891,17,891,33,0,0,4,0],[947,17,947,35,0,0,4,1],[956,17,956,36,0,0,4,0],[956,17,956,36,0,0,4,1],[968,26,968,45,0,0,4,1],[975,26,975,31,0,0,4,0],[975,26,975,31,0,0,4,1],[984,22,984,27,0,0,4,0],[984,22,984,27,0,0,4,1],[984,31,984,38,0,0,4,0],[768,13,768,43,13,0,4,0],[790,13,790,29,34,0,4,0]],"OJPEGPreDecodeSkipScanlines":[[834,9,834,32,0,0,4,0],[834,9,834,32,0,0,4,1],[837,13,837,36,0,0,4,1],[843,17,843,41,0,0,4,0],[843,17,843,41,0,0,4,1],[845,13,846,64,0,0,4,0],[845,13,846,64,0,0,4,1]],"TIFFReadAndRealloc":[[135,17,135,25,0,0,4,0]],"OJPEGReadHeaderInfoSecStreamDqt":[[1690,17,1690,71,0,0,4,0],[1702,17,1702,35,0,0,4,0],[1706,18,1706,23,0,0,4,1]],"OJPEGSubsamplingCorrect":[[1102,13,1102,37,0,0,4,0]],"horAcc8":[[358,13,358,24,0,0,4,0],[364,20,364,26,0,0,4,0],[364,20,364,26,0,0,4,1],[371,18,371,29,0,0,4,0],[378,20,378,26,0,0,4,0],[378,20,378,26,0,0,4,1],[395,22,395,28,0,0,4,0],[316,9,316,16,2,0,4,0],[319,29,319,34,2,0,4,0],[319,29,319,34,2,0,4,1],[328,9,328,15,2,0,4,0],[330,9,330,15,2,0,4,1]],"TIFFReadEncodedStripGetStripSize":[[485,9,485,31,0,0,4,0],[485,9,485,31,0,0,4,1],[487,9,487,32,0,0,4,1],[496,9,496,42,0,0,4,0],[496,9,496,42,0,0,4,1],[501,9,501,15,0,0,4,0],[504,9,504,28,0,0,4,0],[504,9,504,28,0,0,4,1],[507,9,507,23,0,0,4,1],[289,7,289,45,1,0,4,0],[289,7,289,45,1,0,4,1]],"Fax3Unexpected":[[250,22,250,62,1,0,4,1],[250,22,250,62,2,0,4,1]],"PredictorDecodeTile":[[593,13,593,34,0,0,4,0]],"Fax4Decode":[[514,33,514,46,3,0,4,0],[539,33,539,40,3,0,4,0],[552,33,552,47,3,0,4,0],[170,21,170,35,6,0,4,1],[150,21,150,35,57,0,4,1],[326,13,326,22,65,0,4,0],[331,20,331,30,65,0,4,0],[331,34,331,46,65,0,4,0],[333,17,333,27,70,0,4,1],[341,22,341,32,70,0,4,1],[117,21,117,31,74,0,4,0],[166,13,166,28,85,0,4,1],[170,21,170,35,90,0,4,0],[177,21,177,43,90,0,4,0],[166,13,166,28,95,0,4,1],[166,13,166,28,100,0,4,1],[177,21,177,43,100,0,4,0],[268,13,268,38,124,0,4,1],[117,21,117,31,149,0,4,0],[117,21,117,31,150,0,4,1],[117,21,117,31,158,0,4,0],[117,21,117,31,158,0,4,1]],"OJPEGReadHeaderInfoSecStreamSof":[[1801,9,1801,25,0,0,4,0],[1820,13,1820,40,0,0,4,0],[1852,13,1852,39,0,0,4,0]],"horAcc16":[[417,9,417,33,0,0,4,1],[423,9,423,20,0,0,4,0],[423,9,423,20,0,0,4,1],[433,18,433,24,0,0,4,0],[433,18,433,24,0,0,4,1],[316,9,316,16,2,0,4,0],[316,9,316,16,2,0,4,1],[319,29,319,34,2,0,4,0],[319,29,319,34,2,0,4,1],[324,9,324,15,2,0,4,0],[324,9,324,15,2,0,4,1],[326,9,326,15,2,0,4,0],[326,9,326,15,2,0,4,1],[328,9,328,15,2,0,4,0],[328,9,328,15,2,0,4,1],[330,9,330,15,2,0,4,0],[330,9,330,15,2,0,4,1],[332,9,332,15,2,0,4,1]],"LZWDecode":[[521,9,521,31,0,0,4,0],[523,13,523,31,0,0,4,1],[527,13,527,45,0,0,4,1],[644,13,644,21,0,0,4,0],[672,9,672,17,0,0,4,0],[686,14,686,32,0,0,4,0],[687,9,687,25,0,0,4,0],[689,9,689,24,0,0,4,0],[394,21,394,33,5,0,4,0],[396,25,396,41,5,0,4,0],[396,25,396,41,5,0,4,1],[374,17,374,52,19,0,4,1],[387,21,387,37,19,0,4,0],[387,21,387,37,19,0,4,1],[394,21,394,33,19,0,4,1]],"OJPEGDecodeScanlines":[[963,9,963,37,0,0,4,0],[963,9,963,37,0,0,4,1],[973,13,974,50,0,0,4,0],[973,13,974,50,0,0,4,1],[978,14,978,19,0,0,4,0],[978,14,978,19,0,0,4,1]],"OJPEGWriteStream":[[2467,13,2467,25,0,0,4,0]],"TIFFReadRawStripOrTile2":[[666,13,666,21,0,0,4,0]],"Fax3PrematureEOF":[[250,22,250,62,1,0,4,1],[250,22,250,62,2,0,4,1]],"DumpModeDecode":[[77,9,77,28,0,0,4,1],[90,9,90,30,0,0,4,0]],"OJPEGReadHeaderInfoSec":[[1537,13,1537,33,0,0,4,0],[1563,25,1563,52,0,0,4,0],[1591,21,1591,48,0,0,4,1],[1594,21,1594,62,0,0,4,0]],"std_skip_input_data":[[684,9,684,22,0,0,4,0],[686,13,686,56,0,0,4,0],[686,13,686,56,0,0,4,1]],"TIFFRawStripSize64":[[146,9,146,23,0,0,4,0],[146,9,146,23,0,0,4,1]],"OJPEGReadHeaderInfoSecStreamDri":[[1642,9,1642,35,0,0,4,0]],"NeXTPreDecode":[[176,9,176,34,0,0,4,0]],"Fax3DecodeRLE":[[1675,13,1675,37,0,0,4,1],[1680,18,1680,42,0,0,4,0],[1684,17,1684,31,0,0,4,0],[1684,17,1684,31,0,0,4,1],[1684,35,1684,45,0,0,4,0],[1684,35,1684,45,0,0,4,1],[170,21,170,35,35,0,4,1],[117,21,117,31,56,0,4,0]],"LZWPreDecode":[[266,13,266,28,0,0,4,1]],"TIFFReadEncodedStrip":[[525,9,525,38,0,0,4,0],[525,9,525,38,0,0,4,1],[529,9,529,47,0,0,4,0],[529,9,529,47,0,0,4,1],[529,51,529,73,0,0,4,0],[530,9,530,26,0,0,4,0],[530,9,530,26,0,0,4,1],[530,30,530,39,0,0,4,0],[531,9,531,49,0,0,4,0],[533,13,533,79,0,0,4,0],[533,13,533,79,0,0,4,1],[536,13,536,25,0,0,4,0],[536,13,536,25,0,0,4,1],[537,13,537,50,0,0,4,0],[544,9,544,33,0,0,4,0],[544,37,544,55,0,0,4,0],[544,37,544,55,0,0,4,1],[546,9,546,35,0,0,4,0],[546,9,546,35,0,0,4,1],[548,9,548,65,0,0,4,0],[548,9,548,65,0,0,4,1]],"OJPEGReadBufferFill":[[2219,17,2219,48,0,0,4,1]],"NeXTDecode":[[77,9,77,23,0,0,4,0],[109,21,109,31,0,0,4,1],[109,35,109,53,0,0,4,0],[109,35,109,53,0,0,4,1],[152,25,152,32,0,0,4,0],[250,22,250,62,3,0,4,1]],"OJPEGReadHeaderInfoSecTablesAcTable":[[2158,26,2158,78,0,0,4,1],[2162,21,2162,67,0,0,4,0]],"Fax3Decode1D":[[395,21,395,31,4,0,4,0],[401,21,401,35,4,0,4,0],[402,21,402,34,4,0,4,0],[412,17,412,28,4,0,4,0],[414,17,414,31,4,0,4,0],[414,35,414,49,4,0,4,0],[414,35,414,49,4,0,4,1],[326,13,326,22,33,0,4,0],[328,13,328,24,33,0,4,0],[331,20,331,30,33,0,4,0],[331,20,331,30,33,0,4,1],[331,34,331,46,33,0,4,0],[333,17,333,27,33,0,4,0],[335,21,335,27,33,0,4,1],[337,21,337,39,33,0,4,1],[326,13,326,22,34,0,4,0],[117,21,117,31,43,0,4,0],[170,21,170,35,45,0,4,0],[170,21,170,35,50,0,4,1],[177,21,177,43,50,0,4,0],[268,13,268,38,55,0,4,1],[268,13,268,38,58,0,4,1],[268,13,268,38,61,0,4,1],[117,21,117,31,78,0,4,0],[117,21,117,31,81,0,4,0],[117,21,117,31,82,0,4,1]],"TIFFStartStrip":[[1398,9,1398,48,0,0,4,0],[1398,9,1398,48,0,0,4,1],[1400,13,1400,42,0,0,4,0],[1400,13,1400,42,0,0,4,1],[1408,9,1408,40,0,0,4,0],[1408,9,1408,40,0,0,4,1],[1416,13,1416,39,0,0,4,0],[1421,9,1422,10,0,0,4,0],[1421,9,1422,10,0,0,4,1]],"Fax3Extension":[[250,22,250,62,1,0,4,1],[250,22,250,62,2,0,4,1]],"TIFFWriteCheck":[[627,9,627,22,0,0,4,0]],"OJPEGCleanup":[[1054,13,1054,31,0,0,4,0],[1058,13,1058,31,0,0,4,0],[1062,13,1062,32,0,0,4,0],[1066,13,1066,32,0,0,4,0],[1070,13,1070,32,0,0,4,0],[1074,13,1074,32,0,0,4,0],[1082,13,1082,33,0,0,4,0]]},"library_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFStripSize","TIFFRawStripSize","TIFFClose","TIFFOpen","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFWriteEncodedStrip","TIFFClose","TIFFClose"],"critical_calls":["TIFFStreamOpen","TIFFGetField","TIFFGetField","TIFFStripSize","TIFFRawStripSize","TIFFOpen","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFWriteEncodedStrip","TIFFClose","TIFFClose"],"visited":3}
/**/
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* data, size_t size) {
	if(size<0) return 0;

	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);

    // Write data into input_file.
    FILE* in_file = fopen("input_file", "wb");
    if (in_file == NULL) {
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

    // Read image metadata and setup required parameters
    uint32_t width, height;
    TIFFGetField(in_tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(in_tif, TIFFTAG_IMAGELENGTH, &height);
    size_t strip_size = TIFFStripSize(in_tif);
    tmsize_t buffer_size = TIFFRawStripSize(in_tif, 0);

    // Allocate memory for reading image data
    uint8_t* buffer = (uint8_t*)malloc(buffer_size);
    if (!buffer) {
        TIFFClose(in_tif);
        assert_file_closed(&in_file);
	return 0;
    }

    // Read each strip and write to output TIFF
    TIFF* out_tif = TIFFOpen("output_file", "wb");
    uint32_t strip, row;
    for (strip = 0; strip < TIFFNumberOfStrips(in_tif); strip++) {
        TIFFReadEncodedStrip(in_tif, strip, buffer, buffer_size);
        TIFFWriteEncodedStrip(out_tif, strip, buffer, buffer_size);
    }
    
    // Cleanup
    free(buffer);
    TIFFClose(in_tif);
    TIFFClose(out_tif);

    assert_file_name_closed("output_file");
	assert_file_closed(&in_file);
	return 0;
}