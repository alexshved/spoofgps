#ifndef VENSERVER_SUBFRAME_H
#define VENSERVER_SUBFRAME_H
#include <winusb.h>
#define PI 3.1415926535897931
int ParseInt(byte, byte, byte, byte );
typedef struct Subframe
{
    unsigned char data[56];
	int id;
	int page;
	int satelite;
	DWORD TOW;
	DWORD* word;
} TSubframe;

typedef struct Frame
{
    TSubframe **subframe;
    int cnt;
} Tframe;

typedef struct SatSata
{
    	DWORD TOW;
	WORD sf1_WN;							//Transmission Week Number    w3 10bit
	DWORD WN;
	BYTE sf1_ca_or_p_on;					//Code(s) on L2 Channel       w3  2bit
	BYTE sf1_SV_Accuracy; 			        //    w3  4bit     ndex 
	BYTE sf1_SV_Health;                   	//w3  6bit
	WORD sf1_IODC;							//Issue of Data, Clock (IODC) w3  2bit 
	BYTE sf1_dfl2pc;						//Data Flag for L2 P-Code     w4	1bit
	BYTE sf1_TGD;							//Estimated Group Delay Differential (Tgd) 	w7	8bit
	double TGD;
	WORD sf1_TOC;							//SV Clock Correction      (tOC)              w8 16bit
	DWORD TOC;
	BYTE sf1_af2;							//				w9 8bit
	WORD sf1_af1;							//				w9 16bit
	DWORD sf1_af0;							//				w10 22bit
	double AF0;
	double AF1;
	double AF2;
	BYTE sf2_IODE;						//w3 8bit
	double IODE;
	WORD sf2_Crs; 						//w3 16bit
	double CRS;
	WORD sf2_deltaN; 					//w4 16bit
	double DELTA_N;
	DWORD sf2_M0;  						//w4  8bit     msb + w5  24bit    lsb
	double M0;
	WORD  sf2_Cuc;     					// w6  16 bit
	double CUC;
	DWORD sf2_e;						//w6  8 bit   msb + w7  24 bit  lsb
	double E;
	WORD  sf2_Cus;						//w8 16 bit
	double CUS;
	DWORD sf2_sqrtA;					//w8 8 bit   msb + w9 24 bit  lsb
	double SQRT_A;
	WORD sf2_toe;						// w10 16 bit
	double TOE;
	BYTE sf2_FitIntervalFlag;			//w10 1 bit
	BYTE sf2_AODO;						//w10 5 bit

	short  sf3_Cic;			//w3 16 bit
	double CIC;
	int sf3_omega0;		//w3  8 bit +	w4  24 bit
	double OMEGA_0;
	short  sf3_Cis;			//w5 16 bit
	double CIS;

	DWORD sf3_i0;			//w5  8 bit + w6 24 bit
	double I0;
	short  sf3_Crc;			//w7 16 bit
	double CRC;
	DWORD sf3_omega;		//w7  8 bit +	w8 24 bit
	double omega;
	int sf3_omega_dot;   //w9 24 bit
	double OMEGA_DOT;
	BYTE  sf3_IODE;			//w10  8 bit
	WORD  sf3_IDOT;			//w10 14 bit
    double IDOT;
	BYTE sf4_13_AVAILABILITY_INDICATOR;	// - 2 BITS
	BYTE sf4_13_ERD[30];   			 	//	6 bit



	BYTE sf4_18_alfa0;
	BYTE sf4_18_alfa1;
	BYTE sf4_18_alfa2;
	BYTE sf4_18_alfa3;
	BYTE sf4_18_betta0;
	BYTE sf4_18_betta1;
	BYTE sf4_18_betta2;
	BYTE sf4_18_betta3;
	double ION_ALFA[4];
	double ION_BETA[4];

	DWORD sf4_18_A1;		//w6 24 bit
	DWORD sf4_18_A0;		//w7 24 bit +	w8  8 bit lsb
	double A0;
	double A1;

	BYTE  sf4_18_tot;
	double TOT;
	BYTE  sf4_18_WN_t;
	BYTE  sf4_18_delta_t_ls;
	BYTE  sf4_18_WN_lsf;
	BYTE  sf4_18_DN;
	BYTE  sf4_18_delta_t_lsf;

	BYTE sf4_25_SV[32];	 //4 bit
	BYTE sf4_25_SV_HEALF[32];  //6 bit

	BYTE sf5_25_toa;				//   w3	8 bit
	BYTE sf5_25_WNa;               	//   w3 8 bit
//	SV HEALTH SV1-SV24  6 bit

	WORD sf5_24_e;         			//  w3 16 bit
	BYTE sf5_24_toa;  				//       w4 8 bit
	WORD sf5_24_delta;				//		w4 16 bit
	WORD sf5_24_omega_dot;      	// w5 16 bit
	BYTE sf5_24_sv_helth;			//    w5  8 bit
	DWORD sf5_24_sqrtA;     		//  w6 24 bit
	DWORD sf5_24_omega0;			//	w7 24 bit
	DWORD sf5_24_omega;     		//  w8 24 bit
	DWORD sf5_24_M0;      			//    w9 24 bit
	WORD sf5_24_af0;         		//w10 8 bit + 3 bit
	WORD sf5_24_af1; 
} TSatSata;

typedef struct Satelite{
    bool ready[8];
	TSatSata *data;
	Tframe **frame;
	int cntframe;
	int id;
} TSatelite;

typedef struct SateliteList
{
    int size;
    int fripos;
    TSatelite **satelite;
} TSateliteList;
extern TSateliteList *satelitelist;
void calcdata(TSatelite* , TSubframe*);
void CalcParam(TSubframe *);
extern satelite[1024];
#endif