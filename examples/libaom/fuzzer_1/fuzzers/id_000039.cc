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
//<ID> 1453
//<Prompt> ["aom_obu_type_to_string","aom_uleb_encode","aom_codec_encode","aom_codec_set_cx_data_buf","aom_codec_set_option"]
/*<Combination>: [const char *aom_obu_type_to_string(OBU_TYPE type),
    int aom_uleb_encode(uint64_t value, size_t available, uint8_t * coded_value, size_t * coded_size),
    aom_codec_err_t aom_codec_encode(aom_codec_ctx_t * ctx, const aom_image_t * img, aom_codec_pts_t pts, unsigned long duration, aom_enc_frame_flags_t flags),
    aom_codec_err_t aom_codec_set_cx_data_buf(aom_codec_ctx_t * ctx, const aom_fixed_buf_t * buf, unsigned int pad_before, unsigned int pad_after),
    aom_codec_err_t aom_codec_set_option(aom_codec_ctx_t * ctx, const char * name, const char * value)
*/
//<score> 77.625, nr_unique_branch: 137, p_cov: 1
//<Quality> {"diversity":16,"density":9,"covered_api":["aom_obu_type_to_string","aom_uleb_encode","aom_codec_encode","aom_codec_set_cx_data_buf","aom_codec_set_option"],"uncovered_api":[],"unique_branches":{"aom_codec_peek_stream_info":[[62,17,62,22,0]],"encoder_set_option":[[3708,7,3708,18,1],[3708,22,3708,34,1],[3708,38,3708,51,1],[3728,7,3728,15,1],[3734,7,3735,41,1],[3738,14,3739,42,1],[3741,14,3742,42,1],[3744,14,3745,48,1],[3747,14,3748,48,1],[3750,14,3751,42,1],[3753,14,3754,42,1],[3756,14,3757,42,1],[3759,14,3760,42,1],[3762,14,3763,42,1],[3765,14,3766,42,1],[3768,14,3769,42,1],[3771,14,3772,42,1],[3774,14,3775,42,1],[3777,14,3778,48,1],[3780,14,3781,42,1],[3783,14,3784,42,1],[3786,14,3787,42,1],[3797,12,3798,46,1],[3801,14,3803,48,1],[3806,14,3808,48,1],[3812,14,3813,42,1],[3815,14,3816,42,1],[3818,14,3819,48,1],[3822,14,3823,48,1],[3826,14,3827,48,1],[3829,14,3830,42,1],[3832,14,3833,42,1],[3835,14,3836,48,1],[3838,14,3839,48,1],[3841,14,3842,42,1],[3844,14,3845,48,1],[3847,14,3848,42,1],[3850,14,3851,42,1],[3853,14,3854,42,1],[3856,14,3857,42,1],[3859,14,3860,42,1],[3862,14,3863,42,1],[3865,14,3867,48,1],[3870,14,3871,48,1],[3873,14,3874,48,1],[3876,14,3877,42,1],[3879,14,3880,42,1],[3882,14,3883,42,1],[3885,14,3886,42,1],[3888,14,3889,48,1],[3891,14,3892,42,1],[3894,14,3895,48,1],[3897,14,3899,35,1],[3902,14,3904,48,1],[3906,14,3908,29,1],[3910,14,3911,42,1],[3913,14,3914,48,1],[3916,14,3917,42,1],[3919,14,3920,42,1],[3922,14,3923,48,1],[3932,14,3933,42,1],[3935,14,3937,48,1],[3939,14,3940,48,1],[3942,14,3944,48,1],[3946,14,3947,48,1],[3949,14,3950,48,1],[3952,14,3954,48,1],[3957,14,3958,48,1],[3960,14,3961,42,1],[3963,14,3964,48,1],[3966,14,3967,42,1],[3969,14,3970,48,1],[3972,14,3973,48,1],[3975,14,3976,48,1],[3979,14,3980,48,1],[3982,14,3983,48,1],[3985,14,3986,48,1],[3988,14,3990,48,1],[3992,14,3994,48,1],[3996,14,3997,48,1],[3999,14,4001,48,1],[4003,14,4005,48,1],[4007,14,4008,48,1],[4010,14,4011,48,1],[4013,14,4014,48,1],[4016,14,4017,48,1],[4019,14,4020,48,1],[4022,14,4023,48,1],[4025,14,4027,48,1],[4029,14,4030,48,1],[4032,14,4033,42,1],[4035,14,4036,42,1],[4038,14,4039,42,1],[4041,14,4042,48,1],[4044,14,4045,48,1],[4047,14,4048,48,1],[4050,14,4051,48,1],[4053,14,4055,48,1],[4058,14,4059,42,1],[4061,14,4063,48,1],[4066,14,4067,42,1],[4069,14,4070,42,1],[4072,14,4073,48,1],[4075,14,4076,48,1],[4078,14,4079,48,1],[4081,14,4082,48,1],[4086,12,4087,46,1],[4090,14,4091,48,1],[4093,14,4094,48,1],[4098,12,4099,46,1],[4111,14,4113,48,1],[4115,14,4117,48,1],[4119,14,4120,42,1],[4122,14,4123,42,1],[4125,14,4126,42,1],[4129,14,4130,42,1],[4133,14,4134,48,1],[4136,14,4137,48,1],[4139,14,4141,48,1],[4143,14,4144,42,1],[4146,14,4147,48,1],[4149,14,4150,42,1],[4153,14,4154,42,1],[4164,7,4164,26,1],[4169,7,4169,30,0]],"aom_codec_set_option":[[123,36,123,59,1]],"arg_match_helper":[[37,7,37,14,0],[41,7,41,15,1],[41,19,41,36,1],[45,7,45,22,0],[45,7,45,22,1],[45,26,45,67,1],[49,14,49,28,0],[52,9,52,30,0],[53,9,53,60,1],[62,7,62,15,1]]},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_codec_set_option","aom_codec_set_option","aom_codec_set_cx_data_buf","aom_codec_encode","aom_obu_type_to_string","aom_uleb_encode","aom_codec_destroy","aom_codec_destroy"]}
/**/



extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<=74) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




   FILE *in_file = fmemopen((void *)data, size, "rb");
   FILE *out_file = fopen("output_file", "wb");
   int in_fd = fuzz_fileno(in_file);
   int out_fd = fuzz_fileno(out_file);

   // Decode input data
   aom_codec_iface_t *codec_iface = aom_codec_av1_dx();
   aom_codec_ctx_t codec_ctx;
   aom_codec_stream_info_t stream_info;
   aom_codec_dec_cfg_t dec_cfg = { 0, 0, 0 };
   aom_codec_dec_init_ver(&codec_ctx, codec_iface, &dec_cfg, fuzz_int64_t_1, AOM_DECODER_ABI_VERSION);
   aom_codec_peek_stream_info(codec_iface, NULL, 0, &stream_info);
   aom_codec_err_t decode_err = aom_codec_decode(&codec_ctx, data, size, NULL);

   // Encode output data
   aom_codec_ctx_t enc_ctx;
   aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
   aom_codec_enc_cfg_t enc_cfg = { 0 };
   aom_codec_enc_config_default(enc_iface, &enc_cfg, fuzz_uint32_t_2);
   aom_codec_enc_init_ver(&enc_ctx, enc_iface, &enc_cfg, fuzz_int64_t_3, AOM_ENCODER_ABI_VERSION);
   aom_codec_set_option(&enc_ctx, fuzz_str_4, fuzz_str_5);
   aom_codec_set_option(&enc_ctx, fuzz_str_6, fuzz_str_7);
   aom_codec_set_option(&enc_ctx, fuzz_str_8, fuzz_str_9);
   aom_fixed_buf_t output_buf = { 0, 0 };
   aom_codec_set_cx_data_buf(&enc_ctx, &output_buf, 0, fuzz_uint32_t_10);
   aom_codec_pts_t pts = 0;
   aom_enc_frame_flags_t flags = 0;
   aom_codec_encode(&enc_ctx, NULL, pts, fuzz_uint64_t_11, flags);

   // Convert data to string
   const char *obu_type_str = aom_obu_type_to_string(OBU_FRAME);
   uint64_t value = 123;
   size_t code_size;
   uint8_t coded_value[16];
   aom_uleb_encode(value, sizeof(coded_value), coded_value, &code_size);

   // Write output to file
   fwrite(coded_value, code_size, 1, out_file);
   fflush(out_file);

   // Clean up
   aom_codec_destroy(&codec_ctx);
   aom_codec_destroy(&enc_ctx);
   assert_file_closed(&in_file);;
   assert_file_closed(&out_file);;

   assert_file_name_closed("output_file");
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}