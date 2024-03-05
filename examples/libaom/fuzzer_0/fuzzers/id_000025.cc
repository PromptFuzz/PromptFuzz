#include "FDSan.h"
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
//<ID> 956
//<Prompt> ["aom_img_alloc_with_border","aom_uleb_encode_fixed_size","aom_img_alloc","aom_codec_set_frame_buffer_functions","aom_img_remove_metadata","aom_img_plane_height","aom_codec_get_stream_info"]
/*<Combination>: [aom_image_t *aom_img_alloc_with_border(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align, unsigned int size_align, unsigned int border),
    int aom_uleb_encode_fixed_size(uint64_t value, size_t available, size_t pad_to_size, uint8_t * coded_value, size_t * coded_size),
    aom_image_t *aom_img_alloc(aom_image_t * img, aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int align),
    aom_codec_err_t aom_codec_set_frame_buffer_functions(aom_codec_ctx_t * ctx, aom_get_frame_buffer_cb_fn_t cb_get, aom_release_frame_buffer_cb_fn_t cb_release, void * cb_priv),
    void aom_img_remove_metadata(aom_image_t * img),
    int aom_img_plane_height(const aom_image_t * img, int plane),
    aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t * ctx, aom_codec_stream_info_t * si)
*/
//<score> 1.3714287, nr_unique_branch: 2, p_cov: 0.5714286
//<Quality> {"diversity":5,"density":4,"covered_api":["aom_img_alloc","aom_img_remove_metadata","aom_img_plane_height","aom_codec_get_stream_info"],"uncovered_api":["aom_img_alloc_with_border","aom_uleb_encode_fixed_size","aom_codec_set_frame_buffer_functions"],"unique_branches":{"aom_codec_get_stream_info":[[79,7,79,11,0],[19,32,19,35,1]]},"library_calls":["aom_img_alloc","aom_codec_get_stream_info","aom_img_remove_metadata","aom_img_plane_height","aom_img_free"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *data, size_t size) {
	if(size<=0) return 0;

    // Step 4: Create a FILE * variable to read the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Get the file descriptor for reading
    int in_fd = fuzz_fileno(in_file);

    // Step 6: Use the file descriptor or file name as needed

    // Step 1: Create an aom_image_t variable using aom_img_alloc
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 1280, 720, 16);

    // Step 1: Create an aom_codec_stream_info_t variable using aom_codec_get_stream_info
    aom_codec_stream_info_t si;
    aom_codec_get_stream_info(NULL, &si);

    // Step 2: Perform some operations on the aom_image_t and aom_codec_stream_info_t variables
    aom_img_remove_metadata(&img);
    int plane_height = aom_img_plane_height(&img, 0);

    // Step 3: Release the allocated resources
    aom_img_free(&img);

    // Step 4: Close the input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}