#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_127(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_130(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_131(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_0(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_1(input, i_size);
			break;
		case 2:
			return LLVMFuzzerTestOneInput_2(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_3(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_4(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_5(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_6(input, i_size);
			break;
		case 7:
			return LLVMFuzzerTestOneInput_7(input, i_size);
			break;
		case 8:
			return LLVMFuzzerTestOneInput_8(input, i_size);
			break;
		case 9:
			return LLVMFuzzerTestOneInput_9(input, i_size);
			break;
		case 10:
			return LLVMFuzzerTestOneInput_10(input, i_size);
			break;
		case 11:
			return LLVMFuzzerTestOneInput_11(input, i_size);
			break;
		case 12:
			return LLVMFuzzerTestOneInput_12(input, i_size);
			break;
		case 13:
			return LLVMFuzzerTestOneInput_13(input, i_size);
			break;
		case 14:
			return LLVMFuzzerTestOneInput_14(input, i_size);
			break;
		case 15:
			return LLVMFuzzerTestOneInput_15(input, i_size);
			break;
		case 16:
			return LLVMFuzzerTestOneInput_16(input, i_size);
			break;
		case 17:
			return LLVMFuzzerTestOneInput_17(input, i_size);
			break;
		case 18:
			return LLVMFuzzerTestOneInput_18(input, i_size);
			break;
		case 19:
			return LLVMFuzzerTestOneInput_19(input, i_size);
			break;
		case 20:
			return LLVMFuzzerTestOneInput_20(input, i_size);
			break;
		case 21:
			return LLVMFuzzerTestOneInput_21(input, i_size);
			break;
		case 22:
			return LLVMFuzzerTestOneInput_22(input, i_size);
			break;
		case 23:
			return LLVMFuzzerTestOneInput_23(input, i_size);
			break;
		case 24:
			return LLVMFuzzerTestOneInput_24(input, i_size);
			break;
		case 25:
			return LLVMFuzzerTestOneInput_25(input, i_size);
			break;
		case 26:
			return LLVMFuzzerTestOneInput_26(input, i_size);
			break;
		case 27:
			return LLVMFuzzerTestOneInput_27(input, i_size);
			break;
		case 28:
			return LLVMFuzzerTestOneInput_28(input, i_size);
			break;
		case 29:
			return LLVMFuzzerTestOneInput_29(input, i_size);
			break;
		case 30:
			return LLVMFuzzerTestOneInput_30(input, i_size);
			break;
		case 31:
			return LLVMFuzzerTestOneInput_31(input, i_size);
			break;
		case 32:
			return LLVMFuzzerTestOneInput_32(input, i_size);
			break;
		case 33:
			return LLVMFuzzerTestOneInput_33(input, i_size);
			break;
		case 34:
			return LLVMFuzzerTestOneInput_34(input, i_size);
			break;
		case 35:
			return LLVMFuzzerTestOneInput_35(input, i_size);
			break;
		case 36:
			return LLVMFuzzerTestOneInput_36(input, i_size);
			break;
		case 37:
			return LLVMFuzzerTestOneInput_37(input, i_size);
			break;
		case 38:
			return LLVMFuzzerTestOneInput_38(input, i_size);
			break;
		case 39:
			return LLVMFuzzerTestOneInput_39(input, i_size);
			break;
		case 40:
			return LLVMFuzzerTestOneInput_40(input, i_size);
			break;
		case 41:
			return LLVMFuzzerTestOneInput_41(input, i_size);
			break;
		case 42:
			return LLVMFuzzerTestOneInput_42(input, i_size);
			break;
		case 43:
			return LLVMFuzzerTestOneInput_43(input, i_size);
			break;
		case 44:
			return LLVMFuzzerTestOneInput_44(input, i_size);
			break;
		case 45:
			return LLVMFuzzerTestOneInput_45(input, i_size);
			break;
		case 46:
			return LLVMFuzzerTestOneInput_46(input, i_size);
			break;
		case 47:
			return LLVMFuzzerTestOneInput_47(input, i_size);
			break;
		case 48:
			return LLVMFuzzerTestOneInput_48(input, i_size);
			break;
		case 49:
			return LLVMFuzzerTestOneInput_49(input, i_size);
			break;
		case 50:
			return LLVMFuzzerTestOneInput_50(input, i_size);
			break;
		case 51:
			return LLVMFuzzerTestOneInput_51(input, i_size);
			break;
		case 52:
			return LLVMFuzzerTestOneInput_52(input, i_size);
			break;
		case 53:
			return LLVMFuzzerTestOneInput_53(input, i_size);
			break;
		case 54:
			return LLVMFuzzerTestOneInput_54(input, i_size);
			break;
		case 55:
			return LLVMFuzzerTestOneInput_55(input, i_size);
			break;
		case 56:
			return LLVMFuzzerTestOneInput_56(input, i_size);
			break;
		case 57:
			return LLVMFuzzerTestOneInput_57(input, i_size);
			break;
		case 58:
			return LLVMFuzzerTestOneInput_58(input, i_size);
			break;
		case 59:
			return LLVMFuzzerTestOneInput_59(input, i_size);
			break;
		case 60:
			return LLVMFuzzerTestOneInput_60(input, i_size);
			break;
		case 61:
			return LLVMFuzzerTestOneInput_61(input, i_size);
			break;
		case 62:
			return LLVMFuzzerTestOneInput_62(input, i_size);
			break;
		case 63:
			return LLVMFuzzerTestOneInput_63(input, i_size);
			break;
		case 64:
			return LLVMFuzzerTestOneInput_64(input, i_size);
			break;
		case 65:
			return LLVMFuzzerTestOneInput_65(input, i_size);
			break;
		case 66:
			return LLVMFuzzerTestOneInput_66(input, i_size);
			break;
		case 67:
			return LLVMFuzzerTestOneInput_67(input, i_size);
			break;
		case 68:
			return LLVMFuzzerTestOneInput_68(input, i_size);
			break;
		case 69:
			return LLVMFuzzerTestOneInput_69(input, i_size);
			break;
		case 70:
			return LLVMFuzzerTestOneInput_70(input, i_size);
			break;
		case 71:
			return LLVMFuzzerTestOneInput_71(input, i_size);
			break;
		case 72:
			return LLVMFuzzerTestOneInput_72(input, i_size);
			break;
		case 73:
			return LLVMFuzzerTestOneInput_73(input, i_size);
			break;
		case 74:
			return LLVMFuzzerTestOneInput_74(input, i_size);
			break;
		case 75:
			return LLVMFuzzerTestOneInput_75(input, i_size);
			break;
		case 76:
			return LLVMFuzzerTestOneInput_76(input, i_size);
			break;
		case 77:
			return LLVMFuzzerTestOneInput_77(input, i_size);
			break;
		case 78:
			return LLVMFuzzerTestOneInput_78(input, i_size);
			break;
		case 79:
			return LLVMFuzzerTestOneInput_79(input, i_size);
			break;
		case 80:
			return LLVMFuzzerTestOneInput_80(input, i_size);
			break;
		case 81:
			return LLVMFuzzerTestOneInput_81(input, i_size);
			break;
		case 82:
			return LLVMFuzzerTestOneInput_82(input, i_size);
			break;
		case 83:
			return LLVMFuzzerTestOneInput_83(input, i_size);
			break;
		case 84:
			return LLVMFuzzerTestOneInput_84(input, i_size);
			break;
		case 85:
			return LLVMFuzzerTestOneInput_85(input, i_size);
			break;
		case 86:
			return LLVMFuzzerTestOneInput_86(input, i_size);
			break;
		case 87:
			return LLVMFuzzerTestOneInput_87(input, i_size);
			break;
		case 88:
			return LLVMFuzzerTestOneInput_88(input, i_size);
			break;
		case 89:
			return LLVMFuzzerTestOneInput_89(input, i_size);
			break;
		case 90:
			return LLVMFuzzerTestOneInput_90(input, i_size);
			break;
		case 91:
			return LLVMFuzzerTestOneInput_91(input, i_size);
			break;
		case 92:
			return LLVMFuzzerTestOneInput_92(input, i_size);
			break;
		case 93:
			return LLVMFuzzerTestOneInput_93(input, i_size);
			break;
		case 94:
			return LLVMFuzzerTestOneInput_94(input, i_size);
			break;
		case 95:
			return LLVMFuzzerTestOneInput_95(input, i_size);
			break;
		case 96:
			return LLVMFuzzerTestOneInput_96(input, i_size);
			break;
		case 97:
			return LLVMFuzzerTestOneInput_97(input, i_size);
			break;
		case 98:
			return LLVMFuzzerTestOneInput_98(input, i_size);
			break;
		case 99:
			return LLVMFuzzerTestOneInput_99(input, i_size);
			break;
		case 100:
			return LLVMFuzzerTestOneInput_100(input, i_size);
			break;
		case 101:
			return LLVMFuzzerTestOneInput_101(input, i_size);
			break;
		case 102:
			return LLVMFuzzerTestOneInput_102(input, i_size);
			break;
		case 103:
			return LLVMFuzzerTestOneInput_103(input, i_size);
			break;
		case 104:
			return LLVMFuzzerTestOneInput_104(input, i_size);
			break;
		case 105:
			return LLVMFuzzerTestOneInput_105(input, i_size);
			break;
		case 106:
			return LLVMFuzzerTestOneInput_106(input, i_size);
			break;
		case 107:
			return LLVMFuzzerTestOneInput_107(input, i_size);
			break;
		case 108:
			return LLVMFuzzerTestOneInput_108(input, i_size);
			break;
		case 109:
			return LLVMFuzzerTestOneInput_109(input, i_size);
			break;
		case 110:
			return LLVMFuzzerTestOneInput_110(input, i_size);
			break;
		case 111:
			return LLVMFuzzerTestOneInput_111(input, i_size);
			break;
		case 112:
			return LLVMFuzzerTestOneInput_112(input, i_size);
			break;
		case 113:
			return LLVMFuzzerTestOneInput_113(input, i_size);
			break;
		case 114:
			return LLVMFuzzerTestOneInput_114(input, i_size);
			break;
		case 115:
			return LLVMFuzzerTestOneInput_115(input, i_size);
			break;
		case 116:
			return LLVMFuzzerTestOneInput_116(input, i_size);
			break;
		case 117:
			return LLVMFuzzerTestOneInput_117(input, i_size);
			break;
		case 118:
			return LLVMFuzzerTestOneInput_118(input, i_size);
			break;
		case 119:
			return LLVMFuzzerTestOneInput_119(input, i_size);
			break;
		case 120:
			return LLVMFuzzerTestOneInput_120(input, i_size);
			break;
		case 121:
			return LLVMFuzzerTestOneInput_121(input, i_size);
			break;
		case 122:
			return LLVMFuzzerTestOneInput_122(input, i_size);
			break;
		case 123:
			return LLVMFuzzerTestOneInput_123(input, i_size);
			break;
		case 124:
			return LLVMFuzzerTestOneInput_124(input, i_size);
			break;
		case 125:
			return LLVMFuzzerTestOneInput_125(input, i_size);
			break;
		case 126:
			return LLVMFuzzerTestOneInput_126(input, i_size);
			break;
		case 127:
			return LLVMFuzzerTestOneInput_127(input, i_size);
			break;
		case 128:
			return LLVMFuzzerTestOneInput_128(input, i_size);
			break;
		case 129:
			return LLVMFuzzerTestOneInput_129(input, i_size);
			break;
		case 130:
			return LLVMFuzzerTestOneInput_130(input, i_size);
			break;
		case 131:
			return LLVMFuzzerTestOneInput_131(input, i_size);
			break;
		case 132:
			return LLVMFuzzerTestOneInput_132(input, i_size);
			break;
		case 133:
			return LLVMFuzzerTestOneInput_133(input, i_size);
			break;
		case 134:
			return LLVMFuzzerTestOneInput_134(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
