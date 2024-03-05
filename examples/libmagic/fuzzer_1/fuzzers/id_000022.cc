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
//<ID> 1093
//<Prompt> ["vpx_img_wrap","vpx_write_tpl_gop_stats","vpx_codec_encode","vpx_free_tpl_gop_stats","vpx_codec_error","vpx_read_tpl_gop_stats"]
/*<Combination>: [vpx_image_t *vpx_img_wrap(vpx_image_t * img, vpx_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align, unsigned char * img_data),
    vpx_codec_err_t vpx_write_tpl_gop_stats(FILE * tpl_file, const VpxTplGopStats * tpl_gop_stats),
    vpx_codec_err_t vpx_codec_encode(vpx_codec_ctx_t * ctx, const vpx_image_t * img, vpx_codec_pts_t pts, unsigned long duration, vpx_enc_frame_flags_t flags, unsigned long deadline),
    void vpx_free_tpl_gop_stats(VpxTplGopStats * tpl_gop_stats),
    const char *vpx_codec_error(const vpx_codec_ctx_t * ctx),
    vpx_codec_err_t vpx_read_tpl_gop_stats(FILE * tpl_file, VpxTplGopStats * tpl_gop_stats)
*/
//<score> 6.25, nr_unique_branch: 5
//<Quality> {"density":5,"unique_branches":{"compute_skin_map":[[609,7,609,44,0,0,4,1]],"vp8_regulate_q":[[1192,9,1192,63,0,0,4,0]],"vp8_compute_skin_block":[[38,9,38,28,0,0,4,0]],"vp8_alloc_compressor_data":[[1213,7,1213,18,0,0,4,1],[1215,14,1215,27,0,0,4,0]]},"library_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_error","vpx_codec_enc_init_ver","vpx_codec_error","vpx_codec_destroy","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp8_cx","vpx_codec_enc_config_default","vpx_codec_enc_init_ver","vpx_codec_destroy"],"visited":3}
/**/


// Helper function to allocate memory for an image
vpx_image_t * id_000022_allocateImage(unsigned int width, unsigned int height, vpx_img_fmt_t format) {
    vpx_image_t *img = (vpx_image_t *)malloc(sizeof(vpx_image_t));
    if (!img) return NULL;
    
    vpx_img_alloc(img, format, width, height, 1);
    
    return img;
}

// Helper function to free memory allocated for an image
void  id_000022_freeImage(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
        free(img);
    }
}

// Helper function to encode an image using libvpx
void  id_000022_encodeImage(vpx_codec_ctx_t *codec, vpx_image_t *img, FILE *outfile) {
    fseek(outfile, 0, SEEK_SET);
    
    // Initialize the encoder
    vpx_codec_err_t err = vpx_codec_encode(codec, img, 0, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        printf("Error initializing encoder: %s\n", vpx_codec_error(codec));
        return;
    }
    
    // Encode frames
    const vpx_codec_cx_pkt_t *pkt = NULL;
    while ((pkt = vpx_codec_get_cx_data(codec, NULL)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Write encoded data to file
            fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, outfile);
        }
    }
    
    // Flush the encoder
    err = vpx_codec_encode(codec, NULL, -1, 1, 0, VPX_DL_REALTIME);
    if (err != VPX_CODEC_OK) {
        printf("Error flushing encoder: %s\n", vpx_codec_error(codec));
        return;
    }
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
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
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize libvpx
    vpx_codec_iface_t *codec_iface = vpx_codec_vp8_cx();
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err;
    
    err = vpx_codec_enc_config_default(codec_iface, &cfg, fuzz_uint32_t_1);
    if (err != VPX_CODEC_OK) {
        printf("Error initializing codec: %s\n", vpx_codec_error(&codec));
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    cfg.g_w = 640;
    cfg.g_h = 480;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 60;
    cfg.rc_target_bitrate = 5000;
    
    err = vpx_codec_enc_init_ver(&codec, codec_iface, &cfg, fuzz_int64_t_2, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        printf("Error initializing codec: %s\n", vpx_codec_error(&codec));
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read image data from input file
    vpx_image_t *img =  id_000022_allocateImage(cfg.g_w, cfg.g_h, VPX_IMG_FMT_I420);
    size_t bytes_read = fread(img->img_data, 1, img->d_h * img->stride[0], in_file);
    
    if (bytes_read != img->d_h * img->stride[0]) {
        printf("Error reading image data\n");
         id_000022_freeImage(img);
        vpx_codec_destroy(&codec);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Encode the image
     id_000022_encodeImage(&codec, img, out_file);
    
    // Free resources
     id_000022_freeImage(img);
    vpx_codec_destroy(&codec);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}