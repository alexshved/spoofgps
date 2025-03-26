#ifndef VENSERVER_CALCDATA_H
#define VENSERVER_CALCDATA_H
#include <windows.h>
void calc_sf1(TSatelite* , TSubframe*);
void calc_sf2(TSatelite*, TSubframe *);
void calc_sf3(TSatelite*, TSubframe *);
void calc_sf4_13(TSatelite*, TSubframe *);
void calc_sf4_18(TSatelite*, TSubframe *);
void calc_sf4_25(TSatelite*, TSubframe *);
void calc_sf5_24(TSatelite*, TSubframe *);
void calc_sf5_25(TSatelite*, TSubframe *);
typedef  unsigned int uint;
#endif