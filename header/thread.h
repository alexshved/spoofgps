#ifndef VENSERVER_THREAD_H
#define VENSERVER_THREAD_H
#include <winusb.h>
#include <uhd.h>
#include <libusb-1.0/libusb.h>
#include "gpssim.h"
#define MAX_CHAR (100)
// 
// #define TEST_C
// #define TEST
#define NMEAGGA
typedef struct ArgMainThread{
    int argc; 
    char** argv;
} ArgMainThreadT;


int create_Thred(char **, int );


typedef struct ArgGps{
    char navfile[100];
    char umfile[100];
    int nmeaGGA ;
	int umLLH;
    int staticLocationMode ;
    double xyz[USER_MOTION_SIZE][3];
    double llh[3];
    char outfile[300];
    double samp_freq;
    int data_format;
    FILE *fptext ;
    int timeoverwrite;
    datetime_t t0;
    datetime_t tmin;
    datetime_t tmax;
	gpstime_t gmin;
    gpstime_t gmax; 
    gpstime_t g0;
    double duration;
	bool verb;
    ionoutc_t ionoutc;
    int speedcom;
    signed long leapSecond;
    time_t sec_corect;
    int correction; 
    ephem_t eph[EPHEM_ARRAY_SIZE][MAX_SAT];
} ArgGps_t;

typedef struct TimeMessageG{
    int h;
    int m;
    int s;
}TimeMessageG_t;

typedef struct ArgUSRP{
    double freq;
    double rate;
    double gain;
    int readcomport;
    uint64_t total_num_samps;
    
} ARGUSRP_t;

typedef struct BufRinex {
    int wsiz;
    int fl;
    char *buf;
    
}  BufRinex_t, *PBufRinex_t;

typedef struct BufThreds {
    uint64_t wsiz;
    int fl;
    int flr;
    float *buf;
    int fexit;
    
} BufThreds_t, *PBufThreds_t;

typedef struct Args_P {
    ARGUSRP_t *arg_usrt;
    ArgGps_t *arg_gps;
    PBufThreds_t buf;
    BufRinex_t *rinex;
    uhd_usrp_handle *usrp;
    int sped_port;
    char* comport;
    double sec_corect;
    char buf_crd[200];
    char buf_crd_back[200];
    double xyz[USER_MOTION_SIZE][3];
    int flag_buf;
    TimeMessageG_t timemessage;
    int flag_generate;
    int ieph;
   
} Args_P_t, *PArgs_P_t;

void crateRinex(Args_P_t *);

extern int opterr;             
extern int optind;            
extern int optopt;                
extern int optreset;              
extern char *optarg;

#endif