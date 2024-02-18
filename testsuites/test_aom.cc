#include "aom/aom_decoder.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "vector"
#include "fstream"
#include "iostream"
//<Parent> None
//<Combination> ["aom_codec_iface_t * aom_codec_av1_dx()", "    void aom_img_remove_metadata(aom_image_t * img)", "    size_t aom_uleb_size_in_bytes(uint64_t value)", "    aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t * ctx, aom_codec_stream_info_t * si)", "    aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv)", "    size_t aom_uleb_size_in_bytes(uint64_t value)", "    const char * aom_codec_iface_name(aom_codec_iface_t * iface)", "    const char * aom_codec_version_extra_str()", "    aom_codec_iface_t * aom_codec_av1_dx()", "    int aom_img_add_metadata(aom_image_t * img, uint32_t type_, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)", "    const char * aom_codec_build_config()"]
//<Combination>: [aom_codec_iface_t * aom_codec_av1_dx()
//    void aom_img_remove_metadata(aom_image_t * img)
//    size_t aom_uleb_size_in_bytes(uint64_t value)
//    aom_codec_err_t aom_codec_get_stream_info(aom_codec_ctx_t * ctx, aom_codec_stream_info_t * si)
//    aom_codec_err_t aom_codec_decode(aom_codec_ctx_t * ctx, const uint8_t * data, size_t data_sz, void * user_priv)
//    size_t aom_uleb_size_in_bytes(uint64_t value)
//    const char * aom_codec_iface_name(aom_codec_iface_t * iface)
//    const char * aom_codec_version_extra_str()
//    aom_codec_iface_t * aom_codec_av1_dx()
//    int aom_img_add_metadata(aom_image_t * img, uint32_t type_, const uint8_t * data, size_t sz, aom_metadata_insert_flags_t insert_flag)
//    const char * aom_codec_build_config()
//]
//<Prompt> Prompt { kind: Generate } 
//<Mutator> Mutator { op: NoMutation, response: None }
#include <aom/aom.h>
#include <aom/aom_decoder.h>
#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " input_file" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file)
    {
        std::cerr << "Failed to open input file: " << argv[1] << std::endl;
        return 1;
    }

    std::vector<uint8_t> input_data((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
    input_file.close();

    const uint8_t* data = input_data.data();
    size_t data_sz = input_data.size();

    aom_codec_ctx_t codec;
    aom_codec_iface_t* decoder_interface = aom_codec_av1_dx();

    if (aom_codec_dec_init(&codec, decoder_interface, nullptr, 0))
    {
        std::cerr << "Failed to initialize decoder: " << aom_codec_error_detail(&codec) << std::endl;
        return 1;
    }

    aom_codec_stream_info_t stream_info;
    if (aom_codec_get_stream_info(&codec, &stream_info))
    {
        std::cerr << "Failed to get stream info: " << aom_codec_error_detail(&codec) << std::endl;
        aom_codec_destroy(&codec);
        return 1;
    }

    std::cout << "Stream info:\n"
              << "Width: " << stream_info.w << "\n"
              << "Height: " << stream_info.h << "\n"
              << "Is keyframe: " << stream_info.is_kf << "\n"
              << "Number of spatial layers: " << stream_info.number_spatial_layers << "\n"
              << "Number of temporal layers: " << stream_info.number_temporal_layers << "\n"
              << "Is Annex-B: " << stream_info.is_annexb << "\n";

    aom_codec_err_t decode_status = aom_codec_decode(&codec, data, data_sz, nullptr);
    if (decode_status != AOM_CODEC_OK)
    {
        if (decode_status == AOM_CODEC_ERROR)
        {
            std::cerr << "Failed to decode frame: " << aom_codec_error_detail(&codec) << std::endl;
        }
        else
        {
            std::cerr << "Failed to decode frame: " << aom_codec_error(&codec) << std::endl;
        }
        aom_codec_destroy(&codec);
        return 1;
    }

    const char* version = aom_codec_version_extra_str();
    std::cout << "Codec version: " << version << "\n";

    std::cout << "Codec name: " << aom_codec_iface_name(decoder_interface) << "\n";

    aom_image_t* image = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, stream_info.w, stream_info.h, 1);
    if (!image)
    {
        std::cerr << "Failed to allocate image" << std::endl;
        aom_codec_destroy(&codec);
        return 1;
    }

    if (aom_img_add_metadata(image, 1, data, data_sz, AOM_MIF_ANY_FRAME))
    {
        std::cerr << "Failed to add metadata to image" << std::endl;
        aom_img_free(image);
        aom_codec_destroy(&codec);
        return 1;
    }

    aom_img_remove_metadata(image);

    aom_codec_dec_cfg_t config = { 0 };
    config.threads = 1;
    config.w = stream_info.w;
    config.h = stream_info.h;
    config.allow_lowbitdepth = 1;
    const char* build_config = aom_codec_build_config();
    std::cout << "Build config: " << build_config << "\n";

    aom_codec_destroy(&codec);
    aom_img_free(image);
    return 0;
}