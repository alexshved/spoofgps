#ifndef VENSERVER_RINEXOLD_H
#define VENSERVER_RINEXOLD_H
#include <windows.h>
bool checkDataSatelite(TSatelite*);
bool checkRinexCreate();
void MakeRinexFile(char* );
int IntToStr(char*,int);
int FloatToStrR(char*, double, int);
extern time_t last_rinex;
extern int coint_sat;
#endif
