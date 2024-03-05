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
//<ID> 2172
//<Prompt> ["vpx_codec_vp8_cx","vpx_codec_vp8_dx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_img_alloc","vpx_codec_encode","vpx_codec_get_stream_info","vpx_write_tpl_gop_stats","vpx_codec_set_cx_data_buf","vpx_codec_dec_init_ver","vpx_codec_set_frame_buffer_functions","vpx_codec_register_put_slice_cb","vpx_read_tpl_gop_stats","vpx_codec_get_preview_frame","vpx_codec_get_cx_data","vpx_codec_destroy","vpx_img_free"]
/*<Combination>: [vpx_codec_iface_t *vpx_codec_vp8_cx(),
    vpx_codec_iface_t *vpx_codec_vp8_dx(),
    vpx_codec_err_t vpx_codec_enc_config_default(vpx_codec_iface_t * iface, vpx_codec_enc_cfg_t * cfg, unsigned int usage),
    vpx_codec_err_t vpx_codec_enc_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_enc_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_image_t *vpx_img_alloc(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    vpx_codec_err_t vpx_codec_get_stream_info(vpx_codec_ctx_t * ctx, vpx_codec_stream_info_t * si),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_set_cx_data_buf(vpx_codec_ctx_t * ctx, const vpx_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    vpx_codec_err_t vpx_codec_dec_init_ver(vpx_codec_ctx_t * ctx, vpx_codec_iface_t * iface, const vpx_codec_dec_cfg_t * cfg, vpx_codec_flags_t flags, int ver),
    vpx_codec_err_t vpx_codec_set_frame_buffer_functions(vpx_codec_ctx_t * ctx, vpx_get_frame_buffer_cb_fn_t cb_get, vpx_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    vpx_codec_err_t vpx_codec_register_put_slice_cb(vpx_codec_ctx_t * ctx, vpx_codec_put_slice_cb_fn_t cb, void * user_priv),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats),
    const vpx_image_t *vpx_codec_get_preview_frame(vpx_codec_ctx_t * ctx),
    const vpx_codec_cx_pkt_t *vpx_codec_get_cx_data(vpx_codec_ctx_t * ctx, vpx_codec_iter_t * iter),
    vpx_codec_err_t vpx_codec_destroy(vpx_codec_ctx_t * ctx),
    void vpx_img_free(vpx_image_t * img)
*/
//<score> 4, nr_unique_branch: 3
//<Quality> {"density":1,"unique_branches":{"calc_iframe_target_size":[[319,9,319,52,0,0,4,1]],"vp8_change_config":[[1562,7,1562,51,0,0,4,0]],"set_vp8e_config":[[22,23,22,34,1,0,4,1]]},"library_calls":[],"critical_calls":[],"visited":0}
/*Here is an example implementation of the LLVMFuzzerTestOneInput_50 function that uses the libvpx library APIs:

*/


// Include the libvpx headers

// Define the event that the libvpx APIs should achieve
void  id_000050_libvpx_event(const uint8_t *data, size_t size) {
    // Create an instance of the vpx_codec_enc_cfg_t struct
    vpx_codec_enc_cfg_t encodeConfig;
    
    // Set the default encoder configuration
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &encodeConfig, 0);
    
    // Set the configuration parameters
    encodeConfig.g_w = 640;
    encodeConfig.g_h = 480;
    encodeConfig.rc_target_bitrate = 1000000;
    
    // Create an instance of vpx_codec_ctx_t for encoding
    vpx_codec_ctx_t encodeContext;
    
    // Initialize the encoder context with the config
    vpx_codec_enc_init_ver(&encodeContext, vpx_codec_vp8_cx(), &encodeConfig, 0, VPX_ENCODER_ABI_VERSION);
    
    // Allocate an image buffer for encoding
    vpx_image_t *image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, encodeConfig.g_w, encodeConfig.g_h, 1);
    
    // Encode the image data
    vpx_codec_encode(&encodeContext, image, 0, 1, 0, VPX_DL_GOOD_QUALITY);
    
    // Get the encoded data from the encoder context
    const vpx_codec_cx_pkt_t *encodedPacket;
    vpx_codec_iter_t iter = nullptr;
    
    while ((encodedPacket = vpx_codec_get_cx_data(&encodeContext, &iter)) != nullptr) {
        // Write the encoded data to a file
        FILE *outFile = fopen("output_file", "wb");
        fwrite(encodedPacket->data.frame.buf, 1, encodedPacket->data.frame.sz, outFile);
        assert_file_closed(&outFile);;
    }
    
    // Free the allocated resources
    vpx_img_free(image);
    vpx_codec_destroy(&encodeContext);
}

// The LLVMFuzzerTestOneInput_50 function
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Perform the libvpx event with the input data
     id_000050_libvpx_event(data, size);
    
    // Return 0 to indicate success
    return 0;
}