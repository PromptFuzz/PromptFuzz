#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 2160
//<Prompt> ["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_img_get_metadata","aom_img_metadata_free"]
/*<Combination>: [aom_codec_iface_t *aom_codec_av1_cx(),
    aom_codec_err_t aom_codec_enc_init_ver(aom_codec_ctx_t * ctx, aom_codec_iface_t * iface, const aom_codec_enc_cfg_t * cfg, aom_codec_flags_t flags, int ver),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    int aom_img_set_rect(aom_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int border),
    const aom_metadata_t *aom_img_get_metadata(const aom_image_t * img, size_t index),
    void aom_img_metadata_free(aom_metadata_t * metadata)
*/
//<score> 258.6667, nr_unique_branch: 387, p_cov: 0.6666667
//<Quality> {"diversity":10,"density":10,"covered_api":["aom_codec_av1_cx","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect"],"uncovered_api":["aom_img_get_metadata","aom_img_metadata_free"],"unique_branches":{"av1_first_pass_row":[[1223,9,1223,33,0],[1229,11,1229,32,0],[1229,11,1229,32,1]],"get_block_variance_fn":[[198,5,198,19,1],[199,5,199,20,0],[199,5,199,20,1],[200,5,200,20,1],[201,5,201,12,0],[201,5,201,12,1]],"av1_encode":[[3984,7,3984,45,1]],"full_pixel_exhaustive":[[1656,7,1656,26,1],[1656,30,1656,49,1],[1656,53,1656,78,1],[1657,7,1657,25,1],[1673,7,1673,38,1],[1681,7,1681,32,0],[1681,36,1681,55,0],[1684,17,1684,34,0],[1690,11,1690,41,0],[1690,11,1690,41,1],[1694,7,1694,24,0],[1699,7,1699,16,1],[31,23,31,34,0],[31,23,31,34,1],[31,31,31,32,0],[31,31,31,32,1],[31,44,31,45,0],[31,44,31,45,1],[30,23,30,34,0],[30,23,30,34,1],[31,23,31,34,1]],"av1_encode_block_intra":[[789,30,789,53,1],[792,30,792,54,0],[795,30,795,53,1],[798,30,798,54,0]],"exhaustive_mesh_search":[[1570,24,1570,34,0],[1570,24,1570,34,1],[1584,23,1584,35,0],[1584,23,1584,35,1],[1585,25,1585,37,0],[1585,25,1585,37,1],[1587,11,1587,19,0],[1587,11,1587,19,1],[1595,13,1595,29,0],[1595,13,1595,29,1],[1598,23,1598,28,0],[1598,23,1598,28,1],[1605,23,1605,28,0],[1605,23,1605,28,1],[1606,17,1606,35,0],[1606,17,1606,35,1],[1614,23,1614,38,0],[1614,23,1614,38,1],[31,23,31,34,0],[31,23,31,34,1],[31,23,31,34,0],[31,23,31,34,1],[30,23,30,34,0],[30,23,30,34,1],[30,23,30,34,0],[30,23,30,34,1]],"set_good_speed_features_framesize_independent":[[922,7,922,14,1],[922,18,922,80,1],[933,30,933,37,1],[935,45,935,52,1],[955,8,955,15,1],[955,19,955,47,1],[957,14,957,38,1],[958,37,958,44,1]],"encode_block_pass1":[[637,7,637,25,0],[637,7,637,25,1],[639,9,639,26,1]],"encoder_encode":[[2931,11,2931,31,1],[2931,35,2931,60,1],[3008,11,3008,39,1],[3030,11,3030,26,1]],"av1_first_pass":[[1294,7,1294,30,1],[1337,7,1337,51,1],[1345,7,1345,31,0],[1347,18,1347,76,1],[1350,20,1350,80,1],[1380,7,1380,31,0],[1409,7,1409,30,1],[1445,8,1445,41,0],[1446,8,1446,45,0],[1446,8,1446,45,1],[1447,8,1448,68,1],[1467,7,1467,39,1],[1479,7,1479,31,0],[26,33,26,40,1]],"allow_warp":[[41,7,41,24,1],[43,7,43,32,0],[45,7,45,21,1],[47,7,47,37,1],[51,14,51,45,1]],"get_search_range":[[259,10,259,40,1],[30,23,30,34,1]],"av1_init_warp_params":[[63,7,63,42,1],[63,46,63,80,1],[66,7,66,37,1],[68,7,70,50,1]],"is_frame_level_cost_upd_freq_set":[[722,7,722,30,1],[723,7,724,75,0],[723,7,724,75,1],[723,8,723,27,1]],"get_mvpred_compound_var_cost":[[705,7,705,11,1],[709,14,709,25,1]],"mv_err_cost_":[[302,7,302,51,1]],"convolve_2d_facade_single":[[621,7,621,14,0],[621,18,621,25,0]],"av1_encode_strategy":[[1435,31,1435,50,0],[1442,7,1442,57,1],[1524,10,1524,45,1],[1525,10,1525,40,1],[1525,44,1525,75,1],[1526,9,1526,21,1],[1529,16,1529,45,0],[1556,9,1556,45,1],[1557,8,1557,42,1],[1662,30,1662,66,1],[1663,30,1663,73,1]],"av1_dist_wtd_comp_weight_assign":[[667,7,667,19,0]],"av1_get_first_pass_search_site_config":[[274,7,274,74,0]],"update_firstpass_stats":[[933,7,933,26,0]],"av1_lookahead_peek":[[188,7,188,17,1],[195,14,195,23,0],[197,9,197,38,0],[199,11,199,20,1]],"full_pixel_diamond":[[1518,10,1518,27,0],[1518,10,1518,27,1],[1531,9,1531,26,0],[1531,9,1531,26,1],[1537,9,1537,14,0],[1537,9,1537,14,1],[1545,7,1545,16,1]],"get_prediction_error_bitdepth":[[622,7,622,23,1]],"firstpass_inter_prediction":[[753,7,753,61,0],[753,7,753,61,1],[760,9,760,48,0],[760,52,760,72,0],[760,52,760,72,1],[765,11,765,33,0],[765,11,765,33,1],[774,7,774,40,0],[774,7,774,40,1],[774,44,774,64,0],[785,7,785,37,0],[785,7,785,37,1],[785,41,785,75,0],[785,41,785,75,1],[792,7,792,40,0],[792,7,792,40,1],[792,44,792,64,0],[802,7,802,39,0],[810,7,810,39,0],[810,7,810,39,1],[814,9,814,75,0],[814,9,814,75,1],[815,9,815,54,1],[819,16,819,56,0],[820,16,820,73,0],[820,16,820,73,1],[833,9,833,34,0],[30,23,30,34,0],[30,23,30,34,1],[26,33,26,40,1]],"variance16_sse2":[[182,19,182,24,0],[182,19,182,24,1]],"av1_enc_build_inter_predictor":[[114,32,114,49,0],[114,32,114,49,1],[115,9,115,14,1],[121,9,121,38,1]],"mv_err_cost":[[281,5,281,25,0],[282,11,282,17,0],[289,5,289,27,1],[291,5,291,27,1],[293,5,293,26,1],[295,5,295,22,1],[296,5,296,12,1]],"av1_setup_pre_planes":[[729,7,729,18,0],[732,21,732,31,0],[732,21,732,31,1],[30,23,30,34,1]],"mvsad_err_cost":[[320,5,320,25,0],[325,5,325,27,1],[327,5,327,27,1],[329,5,329,26,1],[331,5,331,22,1],[332,5,332,12,1]],"update_mvs_and_sad":[[864,7,864,28,0],[864,7,864,28,1],[868,7,868,22,0],[868,7,868,22,1],[869,9,869,21,1],[871,9,871,23,1]],"get_bsize":[[358,14,358,28,0]],"aom_sad16xNx4d_avx2":[[247,19,247,24,0],[247,19,247,24,1]],"av1_set_frame_size":[[2257,9,2257,20,0],[2262,11,2262,28,1]],"av1_set_mv_search_range":[[211,7,211,35,0],[211,7,211,35,1],[212,7,212,35,0],[212,7,212,35,1],[213,7,213,35,0],[213,7,213,35,1],[214,7,214,35,0],[214,7,214,35,1],[31,23,31,34,0],[31,23,31,34,1],[31,23,31,34,0],[30,23,30,34,0],[30,23,30,34,0],[31,23,31,34,1],[31,23,31,34,1]],"accumulate_mv_stats":[[635,7,635,27,0],[635,7,635,27,1],[639,7,639,47,0],[643,7,643,27,0],[643,7,643,27,1],[644,9,644,19,0],[644,9,644,19,1],[646,16,646,26,0],[649,14,649,34,0],[649,14,649,34,1],[650,9,650,19,0],[650,9,650,19,1],[652,16,652,26,0],[658,7,658,27,0],[658,7,658,27,1],[659,9,659,19,0],[659,9,659,19,1],[661,16,661,26,0],[664,14,664,34,0],[664,14,664,34,1],[665,9,665,19,0],[665,9,665,19,1],[667,16,667,26,0]],"init_mv_cost_params":[[47,7,47,23,0]],"av1_full_pixel_search":[[1797,7,1797,21,1],[1801,7,1801,16,1],[1816,5,1816,21,1],[1820,5,1820,23,1],[1824,5,1824,22,1],[1828,5,1828,18,1],[1832,5,1832,13,1],[1836,5,1836,16,1],[1840,5,1840,16,1],[1844,5,1844,15,0],[1845,5,1845,19,1],[1846,5,1846,17,1],[1847,5,1847,25,1],[1851,5,1851,12,1],[1855,7,1855,23,0],[1856,8,1856,32,0],[1857,7,1857,41,0],[1862,9,1862,29,0],[1862,9,1862,29,1],[1867,7,1867,21,0],[1867,25,1867,53,1],[1875,7,1875,44,1],[1911,7,1911,22,0],[1911,7,1911,22,1],[1926,9,1926,21,0],[1926,9,1926,21,1]],"get_start_mvpred_sad_cost":[[1488,7,1488,40,1]],"av1_fill_mode_rates":[[218,7,218,31,0],[219,17,219,40,0],[219,17,219,40,1],[224,17,224,33,0],[224,17,224,33,1],[225,19,225,38,0],[225,19,225,38,1],[231,17,231,43,0],[231,17,231,43,1],[236,17,236,42,0],[236,17,236,42,1],[237,19,237,43,0],[237,19,237,43,1],[243,17,243,33,0],[243,17,243,33,1],[244,19,244,35,0],[244,19,244,35,1],[250,17,250,33,0],[250,17,250,33,1],[251,19,251,35,0],[251,19,251,35,1],[257,17,257,41,0],[257,17,257,41,1],[262,17,262,40,0],[262,17,262,40,1],[267,17,267,43,0],[267,17,267,43,1],[272,17,272,40,0],[272,17,272,40,1],[277,17,277,38,0],[277,17,277,38,1],[281,17,281,40,0],[281,17,281,40,1],[284,17,284,36,0],[284,17,284,36,1],[287,17,287,36,0],[287,17,287,36,1],[288,11,288,31,0],[288,11,288,31,1],[293,17,293,38,0],[293,17,293,38,1],[299,17,299,36,0],[299,17,299,36,1],[303,25,303,44,0],[303,25,303,44,1],[307,25,307,44,0],[307,25,307,44,1],[311,17,311,40,0],[311,17,311,40,1],[315,17,315,44,0],[315,17,315,44,1]],"diamond_search_sad":[[1371,7,1371,21,1],[1382,7,1382,40,1],[1409,36,1409,45,0],[1409,36,1409,45,1],[1421,11,1421,17,0],[1421,11,1421,17,1],[1422,27,1422,46,0],[1422,27,1422,46,1],[1426,27,1426,32,0],[1426,27,1426,32,1],[1431,27,1431,32,0],[1431,27,1431,32,1],[1432,17,1432,34,0],[1432,17,1432,34,1],[1438,19,1438,36,0],[1438,19,1438,36,1],[1446,27,1446,46,0],[1446,27,1446,46,1],[1450,15,1450,69,0],[1450,15,1450,69,1],[1455,17,1455,34,0],[1455,17,1455,34,1],[1457,19,1457,36,0],[1457,19,1457,36,1],[1470,7,1470,21,1],[1331,9,1331,23,0],[1331,9,1331,23,1],[1339,9,1339,27,0],[1339,9,1339,27,1],[1341,9,1341,23,0],[1341,9,1341,23,1],[1341,27,1341,35,0],[1341,27,1341,35,1],[1343,14,1343,49,1]],"av1_convolve_2d_facade":[[652,7,652,33,1],[652,37,652,63,1],[671,7,671,13,1],[675,14,675,38,1]],"first_pass_motion_search":[[307,7,307,34,1],[318,7,318,24,0],[326,7,326,33,0],[326,7,326,33,1]],"denoise_and_encode":[[846,23,846,60,1],[847,23,847,48,1],[847,52,847,76,1],[852,7,852,20,1]],"av1_check_initial_width":[[2098,7,2098,33,1],[2099,7,2099,55,1],[2100,7,2100,49,1],[2101,7,2101,49,1]],"choose_frame_source":[[371,9,371,43,0]],"av1_make_default_fullpel_ms_params":[[92,7,92,67,1],[112,7,112,38,1],[117,14,117,45,1]],"av1_make_inter_predictor":[[82,7,82,50,0],[84,9,84,39,1]],"raw_motion_error_stdev":[[374,7,374,33,1],[377,15,377,40,0],[377,15,377,40,1],[381,15,381,40,0],[381,15,381,40,1]],"av1_scale_references":[[687,32,687,57,0],[687,32,687,57,1],[689,9,689,66,0],[689,9,689,66,1],[694,11,694,22,1],[702,11,702,28,1],[710,11,710,41,1],[710,45,710,77,1],[795,11,795,35,0]]},"library_calls":["aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_alloc","aom_img_set_rect","aom_codec_encode","aom_codec_get_cx_data","aom_img_free","aom_codec_destroy"]}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<=44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_7, fdp);
	//fuzzer shim end}




    // Create input file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize encoder
    aom_codec_err_t res;
    aom_codec_ctx_t encoder;
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t encoder_cfg;
    aom_codec_enc_config_default(encoder_iface, &encoder_cfg, fuzz_uint32_t_1);
    encoder_cfg.g_w = 1920; // Set width
    encoder_cfg.g_h = 1080; // Set height
    encoder_cfg.rc_target_bitrate = 1000000; // Set target bitrate

    res = aom_codec_enc_init_ver(&encoder, encoder_iface, &encoder_cfg, fuzz_int64_t_2, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Allocate image buffer
    aom_image_t *image = aom_img_alloc(NULL, AOM_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 32);
    if (!image) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        aom_codec_destroy(&encoder);
        assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set image rectangle
    aom_img_set_rect(image, fuzz_uint32_t_3, fuzz_uint32_t_4, encoder_cfg.g_w, encoder_cfg.g_h, 0);

    // Encode frames
    while (!feof(in_file)) {
        if (fread(image->img_data, 1, encoder_cfg.g_w * encoder_cfg.g_h * 3 / 2, in_file) <= 0) {
            break;
        }

        // Encode the frame
        res = aom_codec_encode(&encoder, image, fuzz_int64_t_5, fuzz_uint64_t_6, fuzz_int64_t_7);
        if (res != AOM_CODEC_OK) {
            break;
        }

        // Get compressed data
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&encoder, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Write compressed data to output file
                fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
            }
        }
    }

    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    aom_img_free(image);
    aom_codec_destroy(&encoder);

    assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}