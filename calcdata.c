#include "header/header.h"
#define NTH_BIT(n) (1 << n)

#define TEST_ANY_BITS(x, y) (((x) & (y)) != 0)
#define TEST_ALL_BITS(x, y) (((x) & (y)) == (y))

#define TEST_NTH_BIT(x, n) TEST_ANY_BITS(x, NTH_BIT(n))
DWORD td;
WORD tw;
BYTE tb; 
double to_d;
void calc_sf1(TSatelite* satelite, TSubframe* sf)
{
    //Tframe* f = frame[1];
    //TSubFrame* sf =f->subframe[1];
    satelite->data->sf1_WN = (sf->word[2] >> 20) & 0x3FF; //Transmission Week Number    w3 10bit
    satelite->data->WN = satelite->data->sf1_WN + 2 * 1024;
    satelite->data->sf1_ca_or_p_on =
        (sf->word[2] >> 18) & 0x03; //Code(s) on L2 Channel       w3  2bit
    satelite->data->sf1_SV_Accuracy = (sf->word[2] >> 14) & 0x0F; //    w3  4bit
    satelite->data->sf1_SV_Health = (sf->word[2] >> 8) & 0x3F; //w3  6bit
    BYTE tmp = (((sf->word[2] >> 6) & 0x03) << 8) & 0x0300;
    satelite->data->sf1_IODC =    (sf->word[7] >> 22) &     0xFF; //Issue of Data, Clock (IODC) w3  2bit  старших остальные 8 в w8 total 10bit
    satelite->data->sf1_IODC = satelite->data->sf1_IODC | tmp;
    satelite->data->sf1_dfl2pc = (sf->word[3] >> 23) & 0x01; //Data Flag for L2 P-Code     w4	1bit
    satelite->data->sf1_TGD = (sf->word[6] >> 6) &  0xFF; //Estimated Group Delay Differential (Tgd) 	w7	8bit
	
    tb = satelite->data->sf1_TGD;
    to_d = byteToDouble(tb);
    satelite->data->TGD = to_d / (double)(pow(2.0, 36.0));
    satelite->data->sf1_TOC = (sf->word[7] >> 6) & 0xFFFF; //SV Clock Correction      (tOC)              w8 16bit
    satelite->data->TOC = satelite->data->sf1_TOC * 16;
	satelite->data->sf1_af2 = (sf->word[8] >> 22) & 0xFF; //				w9 8 bit
    satelite->data->sf1_af1 = (sf->word[8] >> 6) & 0xFFFF; //				w9 16bit
    satelite->data->sf1_af0 = (sf->word[9] >> 8) & 0x3FFFFF; //				w10 22bit
    
    td = satelite->data->sf1_af0;
    to_d = dwordToDouble2(tw);
    satelite->data->AF0 = to_d / (double)(pow(2.0, 31.0));
    tw = satelite->data->sf1_af1;
    to_d = wordToDouble(tw);
    
    satelite->data->AF1 = to_d / (double)(pow(2.0, 43.0));
    tb = satelite->data->sf1_af2;
    to_d = byteToDouble(tw);
	satelite->data->AF2 = (double)(to_d) / (double)(pow(2.0, 55.0));
	

}
//------------------------------------------------------------------------------
void calc_sf2(TSatelite *satelite, TSubframe *sf)
{
    satelite->data->sf2_IODE = (sf->word[2] >> 22) & 0xFF; //w3 8bit
    satelite->data->IODE = satelite->data->sf2_IODE;
	satelite->data->sf2_Crs = (sf->word[2] >> 6) & 0xFFFF; //w3 16bit
    tw = satelite->data->sf2_Crs;
    to_d = wordToDouble(tw);
	satelite->data->CRS = to_d / (double)(pow(2.0, 5.0));
	// if(TEST_NTH_BIT(satelite->data->sf2_Crs,15))  
	//  	satelite->data->CRS *= -1;
	satelite->data->sf2_deltaN = (sf->word[3] >> 14);// & 0xFFFF; //w4 16bit
	// writeLogFrameData("delta.txt",&sf->word[3], satelite->id,"word   ",4)  ; 
    tw = satelite->data->sf2_deltaN;
    to_d = wordToDouble(tw);
    writeLogFrameData("delta.txt", &satelite->data->sf2_deltaN, satelite->id, "deltaN ", 2);
    satelite->data->DELTA_N = to_d / (double)(pow(2.0, 43.0));
	
	DWORD tmp =
		(((sf->word[3] >> 6) & 0xFF) << 24) & 0xFF000000; //w4  8bit     msb +
	satelite->data->sf2_M0 = (sf->word[4] >> 6) & 0x0FFFFFF; //w5  24bit    lsb
    
	satelite->data->sf2_M0 = satelite->data->sf2_M0 | tmp;
    td  = satelite->data->sf2_M0;
    to_d = dwordToDouble(td);
    satelite->data->M0 = to_d / (double)(pow(2.0, 31.0));
    
	// if(TEST_NTH_BIT(satelite->data->sf2_M0,23)) 
	//  	satelite->data->M0 *= -1;
	satelite->data->sf2_Cuc = (sf->word[5] >> 14) & 0xFFFF; // w6  16 bit
    tw  = satelite->data->sf2_Cuc;
    to_d = wordToDouble(tw);
	satelite->data->CUC = to_d / (double)(pow(2.0, 29.0));
	// if(TEST_NTH_BIT(satelite->data->sf2_Cuc,15)) 
	// 	satelite->data->CUC *= -1;
	tmp = (((sf->word[5] >> 6) & 0xFF) << 24) & 0xFF000000; //w6  8 bit   msb +
	satelite->data->sf2_e = (sf->word[6] >> 6) & 0x0FFFFFF; //w7  24 bit  lsb
	satelite->data->sf2_e = satelite->data->sf2_e | tmp;
	satelite->data->E = (double)(satelite->data->sf2_e) / (double)(pow(2.0, 33.0));
	satelite->data->sf2_Cus = (sf->word[7] >> 14) & 0xFFFF;
	satelite->data->CUS = (double)(satelite->data->sf2_Cus) / (double)(pow(2.0, 29.0)); //w8 16 bit
	// if(TEST_NTH_BIT(satelite->data->sf2_Cuc,15)) 
	// 	satelite->data->CUS *= -1;
	// }
	tmp = (((sf->word[7] >> 6) & 0xFF) << 24) & 0xFF000000; //w8 8 bit   msb +
	satelite->data->sf2_sqrtA = (sf->word[8] >> 6) & 0x0FFFFFF; //w9 24 bit  lsb
	satelite->data->sf2_sqrtA = satelite->data->sf2_sqrtA | tmp;
	satelite->data->SQRT_A = (double)(satelite->data->sf2_sqrtA) / (double)(pow(2.0, 19.0));
	satelite->data->sf2_toe = (sf->word[9] >> 14) & 0xFFFF;
   	satelite->data->TOE = (double)(satelite->data->sf2_toe) * 16; // w10 16 bit
   	satelite->data->sf2_FitIntervalFlag = (sf->word[9] >> 13) & 0x01; //w10 1 bit
	satelite->data->sf2_AODO = (sf->word[9] >> 8) & 0x1F; //w10 5 bit
}
//------------------------------------------------------------------------------
void calc_sf3(TSatelite *satelite, TSubframe *sf)
{
    char bufp[256];
    satelite->data->sf3_Cic = ParseShort((byte) ((unsigned int) (byte) ((unsigned int) sf->data[23] >> 6) | (unsigned int) sf->data[24] << 2), (byte) ((unsigned int) (byte) ((unsigned int) sf->data[24] >> 6) | (unsigned int) sf->data[25] << 2));
    satelite->data->CIC = (double)((double)(satelite->data->sf3_Cic) * 1.862645149230957E-09);
    satelite->data->sf3_omega0 = ParseInt((byte) ((unsigned int) (byte) ((unsigned int) sf->data[26] >> 6) | (unsigned int) sf->data[27] << 2), (byte) ((unsigned int) (byte) ((unsigned int) sf->data[27] >> 6) | (unsigned int) sf->data[28] << 2), (byte) ((unsigned int) (byte) ((unsigned int) sf->data[28] >> 6) | (unsigned int) sf->data[29] << 2), (byte) ((unsigned int) (byte) ((unsigned int) sf->data[22] >> 6) | (unsigned int) sf->data[23] << 2));
    satelite->data->OMEGA_0 = (double)( (double)satelite->data->sf3_omega0 * 4.6566128730773926E-10 * PI);
	satelite->data->sf3_Cis = ParseShort((byte) ((unsigned int) (byte) ((unsigned int) sf->data[31] >> 6) | (unsigned int) sf->data[32] << 2), (byte) ((uint) (byte) ((uint) sf->data[32] >> 6) | (uint) sf->data[33] << 2));
    satelite->data->CIS = (double)((double)satelite->data->sf3_Cis * 1.862645149230957E-09);
    satelite->data->sf3_Crc = ParseShort((byte) ((uint) (byte) ((uint) sf->data[39] >> 6) | (uint) sf->data[40] << 2), (byte) ((uint) (byte) ((uint) sf->data[40] >> 6) | (uint) sf->data[41] << 2));
    satelite->data->CRC = (double)((double)satelite->data->sf3_Crc * (1.0 / 32.0));
    satelite->data->sf3_i0 = (sf->word[5] >> 4) & 0xFFFFFF; //w5  24bit    lsb
	satelite->data->I0 = (double)(satelite->data->sf3_i0) / (double)(pow(2.0, 31.0));
	satelite->data->sf3_Crc = (sf->word[6] >> 14) & 0xFFFF;
	satelite->data->CRC = (double)(satelite->data->sf3_Crc) / (double)(pow(2.0, 5.0));
	satelite->data->sf3_omega = (sf->word[7] >> 4) & 0xFFFFFF; //w5  24bit    lsb
    td  = satelite->data->sf3_omega;
    to_d = dwordToDouble(td);
    byte b0 = (byte) ((unsigned int) (byte) ((unsigned int) sf->data[46] >> 6) | (unsigned int) sf->data[47] << 2);
    byte b1 = (byte) ((unsigned int) (byte) ((unsigned int) sf->data[47] >> 6) | (unsigned int) sf->data[48] << 2);
    byte b2 = (byte) ((unsigned int) (byte) ((unsigned int) sf->data[48] >> 6) | (unsigned int) sf->data[49] << 2);
    byte b3 = ((int) b2 & 128) > 0 ? 255 : (byte) 0;
    satelite->data->sf3_omega_dot = ParseInt(b0, b1, b2, b3);   
    satelite->data->OMEGA_DOT = satelite->data->sf3_omega_dot * 1.13686837721616E-13 * PI;
	satelite->data->sf3_IODE = (sf->word[9] >> 22) & 0xFF; //w10  8 bit
	satelite->data->IODE = satelite->data->sf3_IODE;
	satelite->data->sf3_IDOT = (sf->word[9] >> 8) & 0x3FFF; //w10 14 bit
    tw  =satelite->data->sf3_IDOT;
        to_d = wordToDouble(td);
	satelite->data->IDOT = to_d / (double)(pow(2.0, 47.0));
}
//------------------------------------------------------------------------------
void calc_sf4_13(TSatelite *satelite, TSubframe *sf)
{
    satelite->data->sf4_13_AVAILABILITY_INDICATOR = (sf->word[2] >> 20) & 0x3; // - 2 BITS
    satelite->data->sf4_13_ERD[0] = (sf->word[2] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[1] = (sf->word[2] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[2] = (((sf->word[2] >> 6) & 0x3) << 4) & 0x30;
    BYTE tmp = (sf->word[3] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[2] = satelite->data->sf4_13_ERD[2] | tmp;

    satelite->data->sf4_13_ERD[3] = (sf->word[3] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[4] = (sf->word[3] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[5] = (sf->word[3] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[6] = (((sf->word[3] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[4] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[6] = satelite->data->sf4_13_ERD[6] | tmp;

    satelite->data->sf4_13_ERD[7] = (sf->word[4] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[8] = (sf->word[4] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[9] = (sf->word[4] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[10] = (((sf->word[4] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[5] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[10] = satelite->data->sf4_13_ERD[10] | tmp;

    satelite->data->sf4_13_ERD[11] = (sf->word[5] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[12] = (sf->word[5] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[13] = (sf->word[5] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[14] = (((sf->word[5] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[6] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[14] = satelite->data->sf4_13_ERD[14] | tmp;

    satelite->data->sf4_13_ERD[15] = (sf->word[6] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[16] = (sf->word[6] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[17] = (sf->word[6] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[18] = (((sf->word[6] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[7] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[18] = satelite->data->sf4_13_ERD[18] | tmp;

    satelite->data->sf4_13_ERD[19] = (sf->word[7] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[20] = (sf->word[7] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[21] = (sf->word[7] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[22] = (((sf->word[7] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[8] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[22] = satelite->data->sf4_13_ERD[22] | tmp;

    satelite->data->sf4_13_ERD[23] = (sf->word[8] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[24] = (sf->word[8] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[25] = (sf->word[8] >> 8) & 0x3F;
    satelite->data->sf4_13_ERD[26] = (((sf->word[8] >> 6) & 0x3) << 4) & 0x30;
    tmp = (sf->word[9] >> 26) & 0xF;
    satelite->data->sf4_13_ERD[26] = satelite->data->sf4_13_ERD[26] | tmp;

    satelite->data->sf4_13_ERD[27] = (sf->word[9] >> 20) & 0x3F;
    satelite->data->sf4_13_ERD[28] = (sf->word[9] >> 14) & 0x3F;
    satelite->data->sf4_13_ERD[29] = (sf->word[9] >> 8) & 0x3F;
}
//------------------------------------------------------------------------------
void calc_sf4_18(TSatelite *satelite, TSubframe *sf)
{
    satelite->data->sf4_18_alfa0 = (sf->word[2] >> 14) & 0xFF;
    satelite->data->sf4_18_alfa1 = (sf->word[2] >> 6) & 0xFF;
	satelite->data->sf4_18_alfa2 = (sf->word[3] >> 22) & 0xFF;
	satelite->data->sf4_18_alfa3 = (sf->word[3] >> 14) & 0xFF;
//	data.sf4_18_alfa2 = (sf->word[3] >> 14) & 0xFF;
//	data.sf4_18_alfa3 = (sf->word[3] >> 6) & 0xFF;
    satelite->data->sf4_18_betta0 = (sf->word[3] >> 6) & 0xFF;
    satelite->data->sf4_18_betta1 = (sf->word[4] >> 22) & 0xFF;
    satelite->data->sf4_18_betta2 = (sf->word[4] >> 14) & 0xFF;
    satelite->data->sf4_18_betta3 = (sf->word[4] >> 6) & 0xFF;

    tb  =satelite->data->sf4_18_alfa0;
    to_d = byteToDouble(tb);

    satelite->data->ION_ALFA[0] = to_d / (double)(pow(2.0, 30.0));
    tb  =satelite->data->sf4_18_alfa1;
    to_d = byteToDouble(tb);
    
    satelite->data->ION_ALFA[1] = to_d / (double)(pow(2.0, 27.0));

    tb  =satelite->data->sf4_18_alfa2;
    to_d = byteToDouble(tb);
    satelite->data->ION_ALFA[2] = to_d / (double)(pow(2.0, 24.0));
    tb  =satelite->data->sf4_18_alfa3;
    to_d = byteToDouble(tb);
    satelite->data->ION_ALFA[3] = to_d / (double)(pow(2.0, 24.0));

    tb  =satelite->data->sf4_18_betta0;
    to_d = byteToDouble(tb);
    satelite->data->ION_BETA[0] = to_d * (double)(pow(2.0, 11.0));
    satelite->data->ION_BETA[1] = (double)(satelite->data->sf4_18_betta1) * (double)(pow(2.0, 14.0));
    tb  =satelite->data->sf4_18_betta2;
    to_d = byteToDouble(tb);
    satelite->data->ION_BETA[2] = to_d * (double)(pow(2.0, 16.0));
    tb  =satelite->data->sf4_18_betta3;
    to_d = byteToDouble(tb);
    satelite->data->ION_BETA[3] = to_d * (double)(pow(2.0, 16.0));

    satelite->data->sf4_18_A1 = (sf->word[5] >> 6) & 0xFFFFFF; //w6 24 bit
    satelite->data->sf4_18_A0 = (((sf->word[6] >> 6) & 0xFFFFFF) << 8) &  0xFFFFFF00; //w7 24 bit +	w8  8 bit lsb
    DWORD tmp = (sf->word[7] >> 22) & 0xFF;
    satelite->data->sf4_18_A0 = satelite->data->sf4_18_A0 | tmp;

    satelite->data->A1 = (double)(satelite->data->sf4_18_A1) / (double)(pow(2.0, 50.0));
    satelite->data->A0 = (double)(satelite->data->sf4_18_A0) / (double)(pow(2.0, 30.0));

    satelite->data->sf4_18_tot = (sf->word[7] >> 14) & 0xFF;
	satelite->data->TOT = (double)(satelite->data->sf4_18_tot) * (double)(pow(2.0, 12.0));

	satelite->data->sf4_18_WN_t = (sf->word[7] >> 6) & 0xFF;
	satelite->data->sf4_18_delta_t_ls = (sf->word[8] >> 22) & 0xFF;
    satelite->data->sf4_18_WN_lsf = (sf->word[8] >> 14) & 0xFF;
    satelite->data->sf4_18_DN = (sf->word[8] >> 6) & 0xFF;
    satelite->data->sf4_18_delta_t_lsf = (sf->word[9] >> 22) & 0xFF;
}
//------------------------------------------------------------------------------
void calc_sf4_25(TSatelite *satelite, TSubframe *sf)
{
    int offset = 0;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV[3 - i] = (sf->word[2] >> (i * 4 + 6)) & 0xF;
    }
    offset += 4;
    for (int i = 0; i < 6; i++) {
        satelite->data->sf4_25_SV[5 - i + offset] = (sf->word[3] >> (i * 4 + 6)) & 0xF;
    }
    offset += 6;
    for (int i = 0; i < 6; i++) {
        satelite->data->sf4_25_SV[5 - i + offset] = (sf->word[4] >> (i * 4 + 6)) & 0xF;
    }
    offset += 6;
    for (int i = 0; i < 6; i++) {
        satelite->data->sf4_25_SV[5 - i + offset] = (sf->word[5] >> (i * 4 + 6)) & 0xF;
    }
    offset += 6;
    for (int i = 0; i < 6; i++) {
        satelite->data->sf4_25_SV[5 - i + offset] = (sf->word[6] >> (i * 4 + 6)) & 0xF;
    }
    offset += 6;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV[3 - i + offset] =
            (sf->word[7] >> (i * 4 + 6 + 6 + 2)) & 0xF;
    }
    offset = 24;
    satelite->data->sf4_25_SV_HEALF[offset] = (sf->word[7] >> (6)) & 0x3F;
    offset = 25;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[8] >> (i * 6 + 6)) & 0x3F;
    }
    offset = 29;
    for (int i = 0; i < 3; i++) {
        satelite->data->sf4_25_SV_HEALF[2 - i + offset] =
            (sf->word[9] >> (i * 6 + 6 + 6)) & 0x3F;
    }
}
//------------------------------------------------------------------------------
void calc_sf5_24(TSatelite *satelite, TSubframe *sf)
{
    satelite->data->sf5_24_e = (sf->word[2] >> 6) & 0xFFFF; //  w3 16 bit
    satelite->data->sf5_24_toa = (sf->word[3] >> 22) & 0xFF; //       w4 8 bit
    satelite->data->sf5_24_delta = (sf->word[3] >> 6) & 0xFFFF; //		w4 16 bit
	satelite->data->sf5_24_omega_dot = (sf->word[4] >> 14) & 0xFFFF; // w5 16 bit
	satelite->data->sf5_24_sv_helth = (sf->word[4] >> 6) & 0xFF; //    w5  8 bit
    satelite->data->sf5_24_sqrtA = (sf->word[5] >> 6) & 0xFFFFFF; //  w6 24 bit
    satelite->data->sf5_24_omega0 = (sf->word[6] >> 6) & 0xFFFFFF; //	w7 24 bit
    satelite->data->sf5_24_omega = (sf->word[7] >> 6) & 0xFFFFFF; //  w8 24 bit
    satelite->data->sf5_24_M0 = (sf->word[8] >> 6) & 0xFFFFFF; //    w9 24 bit
    satelite->data->sf5_24_af0 =
        (((sf->word[9] >> 22) & 0xFF) << 3) & 0xFFF8; //w10 8 bit + 3 bit
    WORD tmp = (sf->word[9] >> 8) & 0x7;
    satelite->data->sf5_24_af0 = satelite->data->sf5_24_af0 | tmp;

    satelite->data->sf5_24_af1 = (sf->word[9] >> 11) & 0x7FF; // w10 11 bit
}
//------------------------------------------------------------------------------
void calc_sf5_25(TSatelite *satelite, TSubframe *sf)
{
    satelite->data->sf5_25_toa = (sf->word[2] >> 14) & 0xFF;
    satelite->data->sf5_25_WNa = (sf->word[2] >> 6) & 0xFF;
    int offset = 0;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[3] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[4] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[5] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[6] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[7] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
    for (int i = 0; i < 4; i++) {
        satelite->data->sf4_25_SV_HEALF[3 - i + offset] =
            (sf->word[8] >> (i * 6 + 6)) & 0x3F;
    }
    offset += 4;
}