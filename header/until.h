#ifndef VENSERVER_UNTIL_H
#define VENSERVER_UNTIL_H
#include <stdio.h>
void writeLog(char*, char*, char*);
void printDoubleBit(char *, char*, int);
void writeLogFrameData(char*, char*, int, char*, int);
void writeLogComon(char*, char*, int, int, char*);
void writeLogMain(char* , char* );
double dwordToDouble(DWORD);
double wordToDouble(WORD);
double byteToDouble(BYTE);
double dwordToDouble2(DWORD);
double dwordToDouble3(DWORD);
void paramprint(char**, int);
void doubleToDate(double , char *,ArgGps_t *);
bool checkDoExit();
void initConfig();
#endif //VENSERVER_UNTIL_H
