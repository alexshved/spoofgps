#ifndef GPSUNIT_H
#define GPSUNIT_H
void subVect(double *, const double *, const double *);
double normVect(const double *);
double dotProd(const double *, const double *);
void codegen(int *, int );
void date2gps(const datetime_t *, gpstime_t *);
void gps2date(const gpstime_t *, datetime_t *);
void xyz2llh(const double *, double *);
void llh2xyz(const double *, double *);
void ltcmat(const double *, double t[3][3]);
void ecef2neu(const double *, double t[3][3], double *);
void neu2azel(double *, const double *);
void satpos(ephem_t , gpstime_t , double *, double *, double *);
void eph2sbf(const ephem_t , const ionoutc_t , unsigned long sbf[5][N_DWRD_SBF], long  );
unsigned long countBits(unsigned long );
unsigned long computeChecksum(unsigned long , int );
int replaceExpDesignator(char *, int );
double subGpsTime(gpstime_t , gpstime_t );
gpstime_t incGpsTime(gpstime_t , double );
int readRinexNavAll(ephem_t eph[][MAX_SAT], ionoutc_t *, const char *);
double ionosphericDelay(const ionoutc_t *, gpstime_t , double *, double *);
void computeRange(range_t *, ephem_t , ionoutc_t *, gpstime_t , double xyz[]);
void computeCodePhase(channel_t *, range_t , double );
int readUserMotion(double xyz[USER_MOTION_SIZE][3], const char *);
int readUserMotionLLH(double xyz[USER_MOTION_SIZE][3], const char *);
int readNmeaGGA(Args_P_t * );
int generateNavMsg(gpstime_t , channel_t *, int );
int checkSatVisibility(ephem_t , gpstime_t , double *, double , double *);
int allocateChannel(channel_t *, ephem_t *, ionoutc_t , gpstime_t , double *, double , long );
void sigint_handler_c(int );
void CorectDate(Args_P_t *);
extern int allocatedSat[MAX_SAT];
extern char kx[11];
extern char ky[11];
extern char kz[11];
extern char ondul[11];
extern char kx_z;
extern char ky_z;

#endif