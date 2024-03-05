#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_decoder.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
//<ID> 196
//<Prompt> ["vpx_img_alloc","vpx_img_set_rect","vpx_codec_enc_config_default","vpx_codec_vp9_cx","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_codec_version_extra_str","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_vp8_cx","vpx_img_free"]
/*<Combination>: [vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    int vpx_img_set_rect(vpx_image_t * img, unsigned int x, unsigned int y, unsigned int w, unsigned int h),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp9_cx(),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_register_put_frame_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_frame_cb_fn_t cb, void * user_priv),
    const char *vpx_codec_version_extra_str(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_iface_t *vpx_codec_vp8_cx(),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 9, nr_unique_branch: 10
//<Quality> {"density":9,"unique_branches":{"ctrl_set_cpuused":[[22,23,22,34,2,0,4,1],[23,23,23,34,3,0,4,1]],"vp9_set_row_mt":[[7040,35,7040,57,0,0,4,1],[7041,8,7041,27,0,0,4,1],[7045,33,7045,52,0,0,4,1],[7052,7,7052,33,0,0,4,0],[7052,37,7052,57,0,0,4,0],[7052,61,7052,77,0,0,4,1]],"update_extra_cfg":[[877,7,877,26,0,0,4,0]],"vpx_codec_control_":[[101,30,101,55,0,0,4,0]]},"library_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_control_","vpx_codec_encode","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_register_put_frame_cb","vpx_img_alloc","vpx_codec_control_","vpx_codec_encode","vpx_img_free","vpx_codec_destroy"],"visited":4}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<32) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for writing encoded data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
   
    // Initialize libvpx
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp9_cx();
    if (!encoder_iface) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    vpx_codec_ctx_t encoder_ctx;
    vpx_codec_enc_cfg_t encoder_cfg;
   
    // Set encoder configuration to default values
    vpx_codec_enc_config_default(encoder_iface, &encoder_cfg, 0);
  
    // Initialize encoder context
    vpx_codec_enc_init_ver(&encoder_ctx, encoder_iface, &encoder_cfg, fuzz_int64_t_1, VPX_ENCODER_ABI_VERSION);
   
    // Register callback function for input frames
    vpx_codec_register_put_frame_cb(&encoder_ctx, NULL, NULL);
    
    // Create output image
    vpx_image_t output_img;
    vpx_img_alloc(&output_img, VPX_IMG_FMT_I420, encoder_cfg.g_w, encoder_cfg.g_h, 32);
    
    // Set encoding parameters
    vpx_codec_control_(&encoder_ctx, VP8E_SET_CPUUSED, 8);
    
    // Encode image
    vpx_codec_encode(&encoder_ctx, &output_img, fuzz_int64_t_2, encoder_cfg.g_timebase.den / encoder_cfg.g_timebase.num, fuzz_int64_t_3, fuzz_uint64_t_4);

    // Release resources
    vpx_img_free(&output_img);
    vpx_codec_destroy(&encoder_ctx);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}